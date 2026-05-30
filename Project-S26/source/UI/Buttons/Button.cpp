#include "Button.h"
#include "SDL3/SDL.h"
#include "../../Tools/ServiceLocator.h"
#include <numbers>
#include <cmath>

Button::Button(float x, float y, float hitAreaW, float hitAreaH, float rotation)
    : UIElement(x, y, rotation), m_fH(hitAreaH), m_fW(hitAreaW),
	m_bIsPressed(false), m_bWasPressedInside(false), m_bIsHovered(false), m_bInteractive(true) {
}

Subscription Button::addClickListener(std::function<void()> callback)
{
	m_clickCallbacks.push_back(callback);
	auto it = --m_clickCallbacks.end();
	return Subscription([this, it]() { m_clickCallbacks.erase(it); });
}

Subscription Button::addHoverEnterListener(std::function<void()> callback)
{
	m_hoverEnterCallbacks.push_back(callback);
	auto it = --m_hoverEnterCallbacks.end();
	return Subscription([this, it]() { m_hoverEnterCallbacks.erase(it); });
}

Subscription Button::addHoverExitListener(std::function<void()> callback)
{
	m_hoverExitCallbacks.push_back(callback);
	auto it = --m_hoverExitCallbacks.end();
	return Subscription([this, it]() { m_hoverExitCallbacks.erase(it); });
}

Subscription Button::addOnHoldListener(std::function<void()> callback)
{
	m_onHoldCallbacks.push_back(callback);
	auto it = --m_onHoldCallbacks.end();
	return Subscription([this, it]() { m_onHoldCallbacks.erase(it); });
}

void Button::onInput(Event& ev)
{
    if (!m_bInteractive) return;
    if (ev.getType() != Event::Type::MOUSE) return;

    const MouseEvent* mev = ev.getMouse();
    MouseEvent::State mouseState = mev->getState();

    m_fLastMouseX = mev->getX();
    m_fLastMouseY = mev->getY();

    bool alreadyHandled = ev.handled();
    bool inside = isMouseInside(mev->getX(), mev->getY());

    if (inside && !alreadyHandled)
    {
        UIElement* eventTarget = ev.getEventTarget();
        bool isTarget = (eventTarget == this);

        if (!m_bIsHovered && isTarget) {
            for (auto& listener : m_hoverEnterCallbacks) listener();
        }
        else if (m_bIsHovered && eventTarget != nullptr && !isTarget) {
            for (auto& listener : m_hoverExitCallbacks) listener();
        }
        else if (m_bWasPressedInside && isTarget) {
            for (auto& listener : m_onHoldCallbacks) listener();
        }

        if (isTarget) ev.handle();
        m_bIsHovered = isTarget;
    }
    else
    {
        if (m_bIsHovered) {
            for (auto& listener : m_hoverExitCallbacks) listener();
        }
        m_bIsHovered = false;
    }

    if (!alreadyHandled)
    {
        UIElement* eventTarget = ev.getEventTarget();
        bool isTarget = (eventTarget == this);

        if (isTarget && mouseState == MouseEvent::State::LEFT_PRESSED)
        {
            m_bIsPressed = true;
            if (m_bIsHovered) {
                ev.handle();
                m_bWasPressedInside = true;
            }
        }
        else if (m_bWasPressedInside && mouseState == MouseEvent::State::LEFT_RELEASED)
        {
            if (m_bIsHovered && isTarget) {
                ev.handle();
                for (auto& listener : m_clickCallbacks) listener();
            }
            m_bIsPressed = false;
            m_bWasPressedInside = false;
        }
    }
    else
    {
        m_bWasPressedInside = false;
        m_bIsPressed = false;
    }
}

void Button::refreshHitBox() {
	Event ev = Event(MouseEvent(MouseEvent::State::MOVED, m_fLastMouseX, m_fLastMouseY));
	onInput(ev);
}
