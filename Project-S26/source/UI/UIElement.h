#pragma once

#include <vector>
#include "../Events/Event.h"

// TODO: CHANGE TO USE MATRIXES
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

protected:
	//If the element is visible,
	//it will be rendered, otherwise it won't be rendered but it will still be able to interact with mouse events
	bool m_bIsVisible;
	//If the element is marked as dead.
	bool m_bDead;
	//The position of the element relative to its container
	float m_fX;
	//The position of the element relative to its container
	float m_fY;
	//The position of the element relative to its container
	float m_fZOrder;
	//The rotation angle of the container in degrees.
	float m_fRotation;

	float m_fW;
	float m_fH;

	//Sort the elements in the container by their Z value (from low to high)
	void sortChildrenByZ();

	//Erase all container childrens
	void clearContainer();
	void cleanUpDeadElements();

	void markForZSort() { m_bNeedsZSort = true; }
	void markForCleanup() { m_bNeedsCleanup = true; }
public:
	UIElement(float x, float y, float width = 0, float height = 0, float rotation = 0, float zOrder = 0);
	virtual ~UIElement();

	float getX() const { return m_fX; };
	void setX(float x) { m_fX = x; };

	float getY() const { return m_fY; };
	void setY(float y) { m_fY = y; };

	float getWidth() const { return m_fW; }
	void setWidth(float w) { m_fW = w; }

	float getHeight() const { return m_fH; }
	void setHeight(float h) { m_fH = h; }

	float getGlobalX() const;
	float getGlobalY() const;
	float getGlobalRotation() const;

	float getZOrder() const { return m_fZOrder; };
	void setZOrder(float z);

	float getRotation() const { return m_fRotation; };
	void setRotation(float rot) { m_fRotation = rot; };

	bool isVisible() const { return m_bIsVisible; };
	void setVisible(bool visible) { m_bIsVisible = visible; };

	void remove();
	bool isMarkedToRemove() const { return m_bDead; }
	
	void addChild(UIElement*);
	//Gets the children vector (used for event handling traversal)
	inline const std::vector<UIElement*>& getChildren() const { return m_children; }

	virtual void update(float dt);
	virtual void render(float parentX, float parentY, float parentRot);
	virtual void onEvent(Event& event);

	//Returns true if the element can be selected as a pointer target.
	virtual bool isPointerTarget() const { return false; }
	//Returns true if the point is inside this element hit area.
	virtual bool hitTest(float x, float y) const { return false; }
	//Finds the event target inside this element subtree (default: self only).
	virtual UIElement* findEventTarget(float x, float y);
	bool isMouseInside(float x, float y);
};