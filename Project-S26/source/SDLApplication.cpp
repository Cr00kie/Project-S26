#include "SDLApplication.h"
#include "Resources/ResourceManager.h"
#include "Events/EventBus.h"
#include "Tweens/TweenManager.h"
#include "Audio/AudioManager.h"
#include "Scenes/SceneManager.h"
#include "Input/InputManager.h"
#include "AppFacadeService.h"
#include "Debug.h"

#include "Scenes/TestScene.h"

#include <SDL3/SDL.h>
#include <memory>
#include <tuple>
#include <iostream>

bool SDLApplication::init()
{
	// Initialize SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_Init Error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Get Device size
	const SDL_DisplayMode* dm = SDL_GetDesktopDisplayMode(SDL_GetPrimaryDisplay());
	int w, h;
	// If no device size, set 1080p as default
	w = dm ? dm->w : 1920;
	h = dm ? dm->h : 1080;

	// Create window
	m_pWindow = SDL_CreateWindow(
		"Project S26 - The Last Braincell",
#if _DEBUG
		// Make screen slightly smaller when debugging
		int(w / 1.5f),
		int(h / 1.5f),
#else
		// Make screen fullscreen when on release
		w,
		h,
#endif
		SDL_WINDOW_RESIZABLE
	);

	if (!m_pWindow) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateWindow Error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Create renderer
	m_pRenderer = SDL_CreateRenderer(m_pWindow, NULL);

	if (!m_pRenderer) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL_CreateRenderer Error", SDL_GetError(), m_pWindow);
		return false;
	}

	SDL_SetRenderVSync(m_pRenderer, 1);

	// Initialize TTF_Library
	if (!TTF_Init())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "TTF Init Error", SDL_GetError(), m_pWindow);
		return false;
	}

	//Initialize audio
	MIX_Init();
	m_sfxMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
	m_musicMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

	if (!m_sfxMixer || !m_musicMixer)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Mixer init error", SDL_GetError(), m_pWindow);
		return false;
	}

	// Initialize global services
	initGlobalServices();

	// Config render
	SDL_SetRenderLogicalPresentation(m_pRenderer, 1920, 1080,
		SDL_RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Subscribe quit event
	EventBus* eb = ServiceLocator::tryGet<EventBus>();
	if (eb != nullptr) {
		eb->subscribe<WindowEvent>("Window_Quitted", [this](const Event&) {
			m_bIsRunning = false;
			});
	}

	m_bIsRunning = true;
	return true;
}

void SDLApplication::run()
{
	INFO("Game loop started\n");

	m_sceneManager->addScene("Front", new TestScene());

	m_sceneManager->activateScene("Front", 0);

	InputContext& context = m_inputManager->actions().createContext("gameplay");
	context.createAction("fire", 0)
		.addBinding(Left)
		.addBinding(SDL_SCANCODE_SPACE)
		.addBinding(SDL_GAMEPAD_BUTTON_SOUTH);

	context.createAction("move", 0, 0.2f)
		.addBinding(SDL_SCANCODE_A, -1)
		.addBinding(SDL_SCANCODE_D, 1)
		.addBinding(SDL_SCANCODE_W, 0, 1)
		.addBinding(SDL_SCANCODE_S, 0, -1)
		.addBinding(SDL_GAMEPAD_AXIS_LEFTX, 1, 0)
		.addBinding(SDL_GAMEPAD_AXIS_LEFTY, 0, 1);

	m_inputManager->actions().pushActiveContext("gameplay");

	uint64_t lastTime = SDL_GetTicks();
	while (m_bIsRunning)
	{
		uint64_t currentTime = SDL_GetTicks();
		float deltaTime = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		handleEvents();

		if (m_inputManager->actions().getContext("gameplay").getAction("fire").justReleased())
		{
			LOG("FIRE PRESSED\n");
		}
		Vec2f val = m_inputManager->actions().getContext("gameplay").getAction("move").readVec2f();
		if (val.getX() || val.getY())
		{
			LOG("MOVE VALUE: " + std::to_string(val.getX()) + ", " +std::to_string(val.getY()) + "\n");
		}
		update(deltaTime);
		render();

		uint64_t frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < m_iFrameDelay)
			SDL_Delay(uint32_t(m_iFrameDelay - frameTime));
	}

	// If run is complete, quit the app automatically
	quit();
}

void SDLApplication::initGlobalServices()
{
	m_debug = new Debug(m_pRenderer);
	ServiceLocator::registerService(m_debug);
	INFO("Debug module initialized\n");

	// Register EventBus as global service
	ServiceLocator::registerService(new EventBus());
	INFO("Event bus initialized\n");

	// Register ResourceManager as global service
	ServiceLocator::registerService(new ResourceManager("assets/AssetsIDs.json", m_pRenderer, m_sfxMixer, m_musicMixer));
	INFO("Resource manager initialized\n");

	ServiceLocator::registerService(new AudioManager(m_sfxMixer, m_musicMixer));
	INFO("Audio manager initialized\n");

	// Register InputManager as global service
	auto const inputManager = new InputManager();
	ServiceLocator::registerService(inputManager);
	m_inputManager = inputManager;
	INFO("Input manager initialized\n");

	// Register TweenManager as global service
	ServiceLocator::registerService(new TweenManager());
	INFO("Tween manager initialized\n");

	// Register AppFacadeService as global service
	ServiceLocator::registerService(new AppFacadeService(this));
	INFO("App facade initialized\n");

	// Register SceneManager as global service
	m_sceneManager = new SceneManager();
	ServiceLocator::registerService(m_sceneManager);
	INFO("Scene manager initialized\n");
}

void SDLApplication::handleEvents() {
	m_inputManager->handleInputs();
	m_sceneManager->processInput(m_inputManager->state());
}

void SDLApplication::update(float deltaTime)
{
	m_sceneManager->update(deltaTime);

	TweenManager* t = ServiceLocator::tryGet<TweenManager>();
	if (t) t->update();
}

void SDLApplication::render()
{
	// Clear screen
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);

	// Render scenes
	m_sceneManager->render();
	m_debug->render();
	
	//m_SceneManager->render();
	SDL_RenderPresent(m_pRenderer);
}

void SDLApplication::quit()
{
	INFO("App quitted\n");
	ServiceLocator::unregister<SceneManager>();

	// Clear Event Bus events
	EventBus* eb = ServiceLocator::tryGet<EventBus>();
	if (eb != nullptr) {
		eb->clearAllEventListeners();
	}
	
	// clear all services in appropiate order
	ServiceLocator::unregister<AudioManager>();
	ServiceLocator::unregister<TweenManager>();
	ServiceLocator::unregister<InputManager>();
	ServiceLocator::unregister<ResourceManager>();
	ServiceLocator::unregister<EventBus>();
	ServiceLocator::unregister<AppFacadeService>();
	ServiceLocator::unregister<Debug>();
	ServiceLocator::clear();

	// Destroy renderer
	if (m_pRenderer) {
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}

	// Destroy window
	if (m_pWindow) {
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}

	// Destroy mixers
	if (m_sfxMixer) {
		MIX_DestroyMixer(m_sfxMixer);
		m_sfxMixer = nullptr;
	}

	if (m_musicMixer) {
		MIX_DestroyMixer(m_musicMixer);
		m_musicMixer = nullptr;
	}

	MIX_Quit();

	// Quit ttf
	TTF_Quit();

	// Quit SDL
	SDL_Quit();
}