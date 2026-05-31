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
		m_fW(width), m_fH(height),
		m_fZOrder(zOrder),
		m_parent(nullptr)
{
	float rot = (rotation * std::numbers::pi_v<float>) / 180.f;
	m_transform = Mat3f::translation({ x, y }) * Mat3f::rotation(rot);
}

UIElement::~UIElement()
{
	clearContainer();
}

Mat3f UIElement::getGlobalTransform() const
{
	if (m_parent) return m_parent->getGlobalTransform() * m_transform;

	return m_transform;
}

Vec2f UIElement::getGlobalPosition() const
{
	return getGlobalTransform() * Vec2f(0, 0);
}

Vec2f UIElement::getPosition() const
{
	return Vec2f(m_transform[2], m_transform[5]);
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

void UIElement::render(const Mat3f& parentTransform)
{
	// If container should resort, resort and reset flag
	if (m_bNeedsZSort)
	{
		sortChildrenByZ();
		m_bNeedsZSort = false;
	}

	// Render elements in order
	for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
	{
		if ((*it)->isVisible()) (*it)->render(parentTransform * m_transform);
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
	// Transform world point into local space using inverse
	Vec2f local = m_transform.inversed() * Vec2f(x, y);
	return local.getX() >= 0 && local.getX() <= m_fW
		&& local.getY() >= 0 && local.getY() <= m_fH;
}
