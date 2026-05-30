#ifndef EVENT_H
#define EVENT_H

#include <variant>

//Window event class,
//used to store the data of window events,
//such as the type of event and the width and height of the window when it was generated.
class WindowEvent {
public:
	//Enum of the different types of window events.
	enum class State {
		CLOSED,
		OPENED,
		RESIZED
	};

private:
	State _state;
	int width;
	int height;

public:
	WindowEvent(State s, int w, int h) : _state(s), width(w), height(h) {};

	const State getState() const { return _state; };
	int getWidth() const { return width; }
	int getHeight() const { return height; }
};

//Key event class,
//used to store the data of key events,
//such as the type of event and the code of the key when it was generated
class KeyEvent {
public:
	//Enum of the different types of key events.
	enum class State {
		PRESSED,
		RELEASED,
		PRESSING
	};

private:
	State _state;
	int _keycode;

public:
	KeyEvent(State s, int k) : _state(s), _keycode(k) {};

	const State getState() const { return _state; }
	int getKeyCode() const { return _keycode; }
};

//Mouse event class,
//used to store the data of mouse events,
//such as the type of event and the position of the mouse when it was generated
class MouseEvent {
public:
	//Enum of the different types of mouse events.
	enum class State {
		MOVED,
		LEFT_PRESSED,
		RIGHT_PRESSED,
		MIDDLE_PRESSED,
		LEFT_PRESSING,
		RIGHT_PRESSING,
		MIDDLE_PRESSING,
		LEFT_RELEASED,
		MIDDLE_RELEASED,
		RIGHT_RELEASED,
		SCROLLED_UP,
		SCROLLED_DOWN
	};

private:
	//type of mouse event.
	State _state;
	//position of the cursor
	float _x, _y;

public:
	MouseEvent(State s, float x, float y) : _state(s), _x(x), _y(y) {};

	const State getState() const { return _state; }
	float getX() const { return _x; }
	float getY() const { return _y; }
};

//Forward declaration
class UIElement;

//Generic event class that can store any of the different types of events of the application,
class Event {
public:
	//Enum of the different types of events of the application,
	//used to identify the type of event stored in the variant
	enum class Type {
		WINDOW,
		KEY,
		MOUSE
	};

private:
	//Whether the event has been handled or not,
	//used to stop sending the event to the lower layers of the stack when it has been handled by one of them
	bool m_bHandled = false;

	//The VisualElement that should handle this event (for mouse events)
	//Used to ensure only the topmost element processes the event
	UIElement* m_pEventTarget = nullptr;

	//Variant that stores the data of the event,
	//it can be a window event, a key event or a mouse event.
	std::variant<WindowEvent, KeyEvent, MouseEvent> _event;

	//Type of the event stored in the variant,
	//used to identify it without needing to use std::get_if and check if it returns nullptr
	Type _type;
public:
	Event(WindowEvent w) : _event(w), _type(Type::WINDOW) {};
	Event(KeyEvent k) : _event(k), _type(Type::KEY) {};
	Event(MouseEvent m) : _event(m), _type(Type::MOUSE) {};

	//It returns the type of the event stored in the variant.
	inline const Type& getType() const { return _type; };

	//It returns data from window events.
	//If the event isnt a window event, returns nullptr
	inline WindowEvent* getWindow() { return std::get_if<WindowEvent>(&_event); }

	//It returns data from mouse events.
	//If the event isnt a mouse event, returns nullptr
	inline MouseEvent* getMouse() { return std::get_if<MouseEvent>(&_event); }

	//It returns data from keyboard events.
	//If the event isnt a keyboard event, returns nullptr
	inline KeyEvent* getKey() { return std::get_if<KeyEvent>(&_event); }

	//It set the event as handled.
	inline void handle() { m_bHandled = true; }

	//It returns true if the event has been handled, false otherwise.
	inline bool handled() const { return m_bHandled; }

	//Sets the event target (which element should handle this event)
	inline void setEventTarget(UIElement* target) { m_pEventTarget = target; }

	//Gets the event target
	inline UIElement* getEventTarget() const { return m_pEventTarget; }
};
#endif