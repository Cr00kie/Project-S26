#include "Slider.h"
#include "../Debug.h"

Slider::Slider(float x, float y, float iniValue, float width, float height, float rotation)
	: UIElement(x, y, width, height, rotation), m_value(iniValue)
{
	m_handle = new Sprite(-width/2, 0, "square", IMAGE, width * 0.1f, height);
	m_bar = new Sprite(0, 0, "square", IMAGE, width, height * 0.2f);
	m_bar->setRGBAModulation({ 70, 70, 70, 255 });

	addChild(m_bar);
	addChild(m_handle);

	m_holdSubscription = m_pointerEvents.onHold.addListener(this, &Slider::onMouseHeld);
}

Slider::~Slider()
{
	m_holdSubscription.unsubscribe();
}

inline void Slider::setValue(float value) 
{ 
	m_value = std::clamp(value, 0.f, 1.f); 

	m_handle->setX(-m_fW/2 + value * m_fW);
}

void Slider::onMouseHeld(MouseEvent& mev)
{
	Vec2f localMousePos = getGlobalTransform().inversed() * Vec2f(mev.getX(), mev.getY());
	float distanceFromStart = localMousePos.getX() - (-m_fW/2);
	float finalValue = distanceFromStart / m_fW;
	setValue(finalValue);
}
