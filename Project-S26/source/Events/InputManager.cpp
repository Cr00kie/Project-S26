#include "InputManager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <utility>
#include "../SDLApplication.h"
#include "../Events/EventBus.h"
#include "../Scenes/SceneManager.h"
#include "../Tools/ServiceLocator.h"

bool InputManager::eventWatcher(void* userdata, SDL_Event* event) {
	InputManager* mngr = static_cast<InputManager*>(userdata);

	std::queue<Event>& m_queue = mngr->m_queue;
	bool& m_bLeftMousePressed = mngr->m_bLeftMousePressed;
	bool& m_bRightMousePressed = mngr->m_bRightMousePressed;
	bool& m_bMiddleMousePressed = mngr->m_bMiddleMousePressed;
	float& m_fLastMouseX = mngr->m_fLastMouseX;
	float& m_fLastMouseY = mngr->m_fLastMouseY;

	auto bus = ServiceLocator::tryGet<EventBus>();
	auto app = ServiceLocator::tryGet<AppFacadeService>();

	switch (event->type) {
		//WINDOWS INPUTS
	case SDL_EVENT_QUIT:
		if (bus) {
			WindowEvent wev(WindowEvent::State::CLOSED, event->window.data1, event->window.data2);
			bus->publish<WindowEvent>("Window_Quitted", wev);
		}
		break;
	case SDL_EVENT_WINDOW_SHOWN:
		if (bus) {
			WindowEvent wev(WindowEvent::State::OPENED, event->window.data1, event->window.data2);
			bus->publish<WindowEvent>("Window_Shown", wev);
		}
		break;
	case SDL_EVENT_WINDOW_RESIZED:
		if (bus) {
			WindowEvent wev(WindowEvent::State::RESIZED, event->window.data1, event->window.data2);
			bus->publish<WindowEvent>("Window_Resized", wev);
		}
		break;

		//KEYBOARD INPUTS
	case SDL_EVENT_KEY_DOWN:
		if (event->key.repeat) m_queue.push(Event(KeyEvent(KeyEvent::State::PRESSING, event->key.key)));
		else m_queue.push(Event(KeyEvent(KeyEvent::State::PRESSED, event->key.key)));
		break;
	case SDL_EVENT_KEY_UP:
		m_queue.push(Event(KeyEvent(KeyEvent::State::RELEASED, event->key.key)));
		break;

		//MOUSE INPUTS
	case SDL_EVENT_MOUSE_BUTTON_UP:
		//Convierto la posición del ratón en posición relativa al LogicalRenderer
		if (app) {
			SDL_RenderCoordinatesFromWindow(&app->getRenderer(), event->motion.x, event->motion.y, &event->motion.x, &event->motion.y);
			if (event->button.button == SDL_BUTTON_LEFT) {
				m_queue.push(Event(MouseEvent(MouseEvent::State::LEFT_RELEASED, event->motion.x, event->motion.y)));
				m_bLeftMousePressed = false;
			}
			else if (event->button.button == SDL_BUTTON_RIGHT) {
				m_queue.push(Event(MouseEvent(MouseEvent::State::RIGHT_RELEASED, event->motion.x, event->motion.y)));
				m_bRightMousePressed = false;
			}
			else {
				m_queue.push(Event(MouseEvent(MouseEvent::State::MIDDLE_RELEASED, event->motion.x, event->motion.y)));
				m_bMiddleMousePressed = false;
			}
		}
		break;
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
		if (app) {
			//Convierto la posición del ratón en posición relativa al LogicalRenderer
			SDL_RenderCoordinatesFromWindow(&app->getRenderer(), event->motion.x, event->motion.y, &event->motion.x, &event->motion.y);
			if (event->button.button == SDL_BUTTON_LEFT) {
				m_queue.push(Event(MouseEvent(MouseEvent::State::LEFT_PRESSED, event->motion.x, event->motion.y)));
				m_bLeftMousePressed = true;
			}
			else if (event->button.button == SDL_BUTTON_RIGHT) {
				m_queue.push(Event(MouseEvent(MouseEvent::State::RIGHT_PRESSED, event->motion.x, event->motion.y)));
				m_bRightMousePressed = true;
			}
			else {
				m_queue.push(Event(MouseEvent(MouseEvent::State::MIDDLE_PRESSED, event->motion.x, event->motion.y)));
				m_bMiddleMousePressed = true;
			}
		}
		break;
	case SDL_EVENT_MOUSE_MOTION:
		if (app) {
			//Convierto la posición del ratón en posición relativa al LogicalRenderer
			SDL_RenderCoordinatesFromWindow(&app->getRenderer(), event->motion.x, event->motion.y, &event->motion.x, &event->motion.y);
			m_queue.push(Event(MouseEvent(MouseEvent::State::MOVED, event->motion.x, event->motion.y)));
			//Update last mouse position
			m_fLastMouseX = event->motion.x;
			m_fLastMouseY = event->motion.y;
		}
		break;
	case SDL_EVENT_MOUSE_WHEEL:
		if (app) {
			//Convierto la posición del ratón en posición relativa al LogicalRenderer
			SDL_RenderCoordinatesFromWindow(&app->getRenderer(), event->motion.x, event->motion.y, &event->motion.x, &event->motion.y);
			if (event->wheel.y < 0) m_queue.push(Event(MouseEvent(MouseEvent::State::SCROLLED_DOWN, event->wheel.mouse_x, event->wheel.mouse_y)));
			else m_queue.push(Event(MouseEvent(MouseEvent::State::SCROLLED_UP, event->wheel.mouse_x, event->wheel.mouse_y)));
		}
		break;
	default: break;
	}

	return true;
}

InputManager::InputManager() {
	SDL_AddEventWatch(eventWatcher, this);
}

InputManager::~InputManager() {
	SDL_RemoveEventWatch(eventWatcher, this);
}

void
InputManager::handleInputs() {
	//It dumps the events collected by the OS so far into the SDL event queue
	//and sends them for review by calling the watcher.
	SDL_PumpEvents();

	//It sends all the events stored in the queue to the SceneManager,
	//starting from the top of the stack and going down until the bottom of the stack is reached.
	auto scenes = ServiceLocator::tryGet<SceneManager>();

	while (!m_queue.empty() && scenes) {
		scenes->onEvent(m_queue.front());
		m_queue.pop();
	}

	//It sends mouse pressing events if the mouse buttons are currently pressed,
	//so they are sent allways while the button is pressed,
	//not only when it is first pressed
	if (m_bLeftMousePressed) m_queue.push(Event(MouseEvent(MouseEvent::State::LEFT_PRESSING, m_fLastMouseX, m_fLastMouseY)));
	if (m_bRightMousePressed) m_queue.push(Event(MouseEvent(MouseEvent::State::RIGHT_PRESSING, m_fLastMouseX, m_fLastMouseY)));
	if (m_bMiddleMousePressed) m_queue.push(Event(MouseEvent(MouseEvent::State::MIDDLE_PRESSING, m_fLastMouseX, m_fLastMouseY)));
}
