#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "../Events/ElementEventSystem.h"
#include "../Math/Mat3.h"

class UIElement
{
private:
	//If the container should clean its dead elements in the next frame
	bool m_bNeedsCleanup = false;
	//If the container should resort its elements in the next frame
	bool m_bNeedsZSort = false;

	//The container that the element belongs to,
	//it can be nullptr if the element is not added to any container
	UIElement* m_parent;
	//The elements contained in the container,
	//they will be rendered in the order of their Z value (from low to high)
	std::vector<UIElement*> m_children;
	void setParent(UIElement* parent) { m_parent = parent; }

	bool m_bWasPressedInside;
	bool m_bIsHovered;
	bool m_bInteractive; // TODO: Make possible to set to not interactive

protected:
	//If the element is visible,
	//it will be rendered, otherwise it won't be rendered but it will still be able to interact with mouse events
	bool m_bIsVisible;
	//If the element is marked as dead.
	bool m_bDead;

	Mat3f m_transform;
	Vec2f m_scale;
	float m_fW, m_fH;
	float m_fZOrder;

	//Sort the elements in the container by their Z value (from low to high)
	void sortChildrenByZ();

	//Erase all container childrens
	void clearContainer();
	void cleanUpDeadElements();

	void markForZSort() { m_bNeedsZSort = true; }
	void markForCleanup() { m_bNeedsCleanup = true; }

public:
	ElementEventSystem m_userEvents;
	UIElement(float x, float y, float width = 0, float height = 0, float rotation = 0, float scaleX = 1, float scaleY = 1, float zOrder = 0);
	virtual ~UIElement();

	float getX() const { return m_transform[2]; };
	void setX(float x) { m_transform[2] = x; };

	float getY() const { return m_transform[5]; };
	void setY(float y) { m_transform[5] = y; };

	float getWidth() const { return m_fW; }
	void setWidth(float w) { m_fW = w; }

	float getHeight() const { return m_fH; }
	void setHeight(float h) { m_fH = h; }

	Vec2f getScale() const { return m_scale; }
	float getScaleX() const { return m_scale.getX(); }
	float getScaleY() const { return m_scale.getY(); }
	void setScale(Vec2f scale) { m_scale = scale; }
	void setScale(float scale) { m_scale.set(scale, scale); }
	void setScale(float scaleX, float scaleY) { m_scale.set(scaleX, scaleY); }
	void setScaleX(float scaleX) { m_scale.setX(scaleX); }
	void setScaleY(float scaleY) { m_scale.setY(scaleY); }

	Mat3f getGlobalTransform() const;
	Vec2f getGlobalPosition() const;
	Vec2f getPosition() const;
	void setPosition(float x, float y) { m_transform[2] = x; m_transform[5] = y; }

	float getZOrder() const { return m_fZOrder; };
	void setZOrder(float z);

	float getRotation() const { return m_transform.getRotation(); };
	float getAngle() const { return m_transform.getAngle(); };
	void setRotation(float rot) { m_transform = Mat3f::translation(getPosition()) * Mat3f::rotation(rot); };
	void rotate(float radians) { m_transform *= Mat3f::rotation(radians); };

	bool isVisible() const { return m_bIsVisible; };
	void setVisible(bool visible) { m_bIsVisible = visible; };

	bool isInteractive() const { return m_bInteractive; }
	void setInteractive(bool interactive) { m_bInteractive = interactive; }

	void remove();
	bool isMarkedToRemove() const { return m_bDead; }
	
	void addChild(UIElement*);
	//Gets the children vector (used for event handling traversal)
	inline const std::vector<UIElement*>& getChildren() const { return m_children; }

	virtual void update(float dt);
	virtual void render(const Mat3f& parentTransform);
	virtual void onEvent(Event& event);

	void HandleMouseEvent(Event& ev);

	//Finds the event target inside this element subtree (default: self only).
	virtual UIElement* findEventTarget(float x, float y);
	bool isMouseInside(float x, float y);
};