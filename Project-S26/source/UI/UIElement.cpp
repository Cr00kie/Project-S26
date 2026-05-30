#include "UIElement.h"
#include "SDL3/SDL.h"
#include <algorithm>
#include <numbers>
#include <cmath>

// Helper function used to order visual elements inside a container. Determines the order VE will follow inside it.
bool UIElementsZOrdering(UIElement* lhs, UIElement* rhs)
{
	return lhs->getZOrder() < rhs->getZOrder();
}

void UIElement::sortChildrenByZ()
{
	// MUST BE STABLE SORT, slower but is necessary to keep the previous order of elements (to avoid clipping like visual errors)
	std::stable_sort(m_children.begin(), m_children.end(), UIElementsZOrdering);
}

void UIElement::clearContainer()
{
	// Destroy all visual elements inside container
	for (auto& e : m_children) delete e;
}

void UIElement::cleanUpDeadElements()
{
	auto result = std::remove_if(m_children.begin(), m_children.end(), [](UIElement* e)
		{
			if (e->isMarkedToRemove())
			{
				delete e;
				return true;
			}

			return false;
		});

	m_children.erase(result, m_children.end());
}

UIElement::UIElement(float x, float y, float width, float height, float rotation, float zOrder)
	:	m_bIsVisible(true),
		m_bDead(false),
		m_fX(x), m_fY(y), 
		m_fW(width), m_fH(height),
		m_fRotation(rotation),
		m_fZOrder(zOrder),
		m_parent(nullptr)
{
}

UIElement::~UIElement()
{
	clearContainer();
}

float UIElement::getGlobalX() const
{
	if (!m_parent) return m_fX;

	float rad = m_parent->getGlobalRotation() * std::numbers::pi_v<float> / 180.0f;

	float rotatedX = m_fX * cos(rad) - m_fY * sin(rad);
	return (rotatedX + m_parent->getGlobalX());
}

float UIElement::getGlobalY() const
{
	if (!m_parent) return m_fY;

	float rad = m_parent->getGlobalRotation() * std::numbers::pi_v<float> / 180.0f;

	float rotatedY = m_fX * std::sin(rad) + m_fY * std::cos(rad);
	return (rotatedY + m_parent->getGlobalY());
}

float UIElement::getGlobalRotation() const
{
	if (!m_parent) return m_fRotation;
	return m_fRotation + m_parent->getGlobalRotation();
}

void UIElement::setZOrder(float z)
{
	m_fZOrder = z;

	// Mark container to resort it self whenever it can
	if (m_parent) m_parent->markForZSort();
}

void UIElement::remove()
{
	m_bDead = true;

	if(m_parent) m_parent->markForCleanup();
}

void UIElement::addChild(UIElement* element)
{
	// Add visual element inserted in order
	// Another option would be to add it to the end and then mark the container to resort afterwards.
	auto it = std::lower_bound(m_children.begin(), m_children.end(), element, UIElementsZOrdering);
	m_children.insert(it, element);

	element->setParent(this);
}

void UIElement::update(float dt)
{
	if (m_bNeedsCleanup)
	{
		cleanUpDeadElements();
		m_bNeedsCleanup = false;
	}

	// Update elements in order
	for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
	{
		(*it)->update(dt);
	}
}

void UIElement::render(float parentX, float parentY, float parentRot)
{
	float x = m_fX + parentX;
	float y = m_fY + parentY;
	float rot = m_fRotation + parentRot;

	// If container should resort, resort and reset flag
	if (m_bNeedsZSort)
	{
		sortChildrenByZ();
		m_bNeedsZSort = false;
	}

	// Render elements in order
	for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
	{
		if ((*it)->isVisible()) (*it)->render(x, y, rot);
	}
}

void UIElement::onEvent(Event& event)
{
}

UIElement* UIElement::findEventTarget(float x, float y)
{
	if (isMarkedToRemove()) return nullptr;
	
	if (m_bNeedsZSort) sortChildrenByZ();

	UIElement* target = nullptr;
	
	for (auto element : m_children)
	{
		target = element->findEventTarget(x, y);
		if (target) break;
	}

	if (!target && isMouseInside(x, y))
	{
		target = this;
	}

	return target;
}

bool UIElement::isMouseInside(float x, float y)
{
	float rad = getGlobalRotation() * std::numbers::pi_v<float> / 180.0f;

	// Mouse relative to the button's center
	float rx = x - getGlobalX();
	float ry = y - getGlobalY();

	//Rotate the point by the negative angle of the button
	float localX = rx * cos(-rad) - ry * sin(-rad);
	float localY = rx * sin(-rad) + ry * cos(-rad);

	SDL_FPoint point = { localX, localY };
	SDL_FRect area = { -m_fW / 2 ,-m_fH / 2, m_fW, m_fH };

	// Check if mouse clicked hit area
	bool ret = SDL_PointInRectFloat(&point, &area);
	return ret;
}
