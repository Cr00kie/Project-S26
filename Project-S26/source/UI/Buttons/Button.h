#pragma once

#include <functional>
#include <string>
#include <memory>

#include "../UIElement.h"
#include "../../Events/Event.h"
#include "../../Events/EventBus.h"

class Subscription;
class Scene;

//Button is a container that represents a clickable area in the UI.
//It can detect mouse events such as clicks and hovers, and it can have callbacks for those events.
//The button itself doesn't have any visual representation,
//but it can contain other VisualElements as its children to create the visual appearance of the button.
class Button : public UIElement {
public:
	// Creates an Button
	// Arguments:
	//		- container: the container that will contain the button (scene or another container)
	//		- x: x position of the button
	//		- y: y position of the button
	//		- hitAreaW: width of the button clickable area
	//		- hitAreaH: height of the button clickable area
	//		- rotation: rotation of the button
	Button(float x, float y, float hitAreaW, float hitAreaH, float rotation = 0);

	//It adds a callback to the list of callbacks of the click event of the button.
	Subscription addClickListener(std::function<void()> callback);
	//It adds a callback to the list of callbacks of the hover enter event of the button.
	Subscription addHoverEnterListener(std::function<void()> callback);
	//It adds a callback to the list of callbacks of the hover exit event of the button.
	Subscription addHoverExitListener(std::function<void()> callback);
	//It adds a callback to the list of callbacks of the holding event of the button.
	Subscription addOnHoldListener(std::function<void()> callback);
	//Set the button to handle events or not.
	void setInteractive(bool interactive) { m_bInteractive = interactive; }

	//It refreshes the hit box of the button,
	//by sending a fake mouse move event with the last mouse coordinates,
	virtual void refreshHitBox();

private:

	//Lists of callbacks for the different events of the button
	std::list<std::function<void()>> m_clickCallbacks;
	std::list<std::function<void()>> m_hoverEnterCallbacks;
	std::list<std::function<void()>> m_hoverExitCallbacks;
	std::list<std::function<void()>> m_onHoldCallbacks;

	//The dimensions of the hit area of the button,
	//used to detect mouse events on the button
	float m_fW;
	float m_fH;

	//State variables of the button,
	//used to detect the different events of the button
	bool m_bIsPressed;
	bool m_bWasPressedInside;
	bool m_bIsHovered;
	bool m_bInteractive;
	
	//The last mouse coordinates received in the button
	float m_fLastMouseX = 0.0f;
	float m_fLastMouseY = 0.0f;

	//Event handler,
	//called by the Scene when it receives an event from the input manager
	virtual void onEvent(Event& ev);
};