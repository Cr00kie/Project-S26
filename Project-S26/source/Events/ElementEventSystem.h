#pragma once

#include <list>
#include <functional>

#include "Event.h"
#include "EventChannel.h"


class ElementEventSystem
{
public:
	ElementEventSystem() = default;
	ElementEventSystem(const ElementEventSystem&) = delete;
	ElementEventSystem& operator=(const ElementEventSystem&) = delete;
	ElementEventSystem(ElementEventSystem&&) = delete;
	ElementEventSystem& operator=(ElementEventSystem&&) = delete;

	//Lists of callbacks for the different events of the button
	EventChannel<MouseEvent> onClick;
	EventChannel<MouseEvent> onHoverEnter;
	EventChannel<MouseEvent> onHoverExit;
	EventChannel<MouseEvent> onHold;
};