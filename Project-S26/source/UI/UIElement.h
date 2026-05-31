#pragma once

#include <vector>
#include "../Events/Event.h"
#include "../Math/Mat3.h"

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

	Mat3f m_transform;
	// Vec2f m_scale; USE THIS IF WE EVER WANT TO SCALE SHIT
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
	UIElement(float x, float y, float width = 0, float height = 0, float rotation = 0, float zOrder = 0);
	virtual ~UIElement();

	float getX() const { return m_transform[2]; };
	void setX(float x) { m_transform[2] = x; };

	float getY() const { return m_transform[5]; };
	void setY(float y) { m_transform[5] = y; };

	float getWidth() const { return m_fW; }
	void setWidth(float w) { m_fW = w; }

	float getHeight() const { return m_fH; }
	void setHeight(float h) { m_fH = h; }

	Mat3f getGlobalTransform() const;
	Vec2f getGlobalPosition() const;
	Vec2f getPosition() const;

	float getZOrder() const { return m_fZOrder; };
	void setZOrder(float z);

	float getRotation() const { return m_transform.getRotation(); };
	float getAngle() const { return m_transform.getAngle(); };
	void setRotation(float rot) { m_transform = Mat3f::translation(getPosition()) * Mat3f::rotation(rot); };
	void rotate(float radians) { m_transform *= Mat3f::rotation(radians); };

	bool isVisible() const { return m_bIsVisible; };
	void setVisible(bool visible) { m_bIsVisible = visible; };

	void remove();
	bool isMarkedToRemove() const { return m_bDead; }
	
	void addChild(UIElement*);
	//Gets the children vector (used for event handling traversal)
	inline const std::vector<UIElement*>& getChildren() const { return m_children; }

	virtual void update(float dt);
	virtual void render(const Mat3f& parentTransform);
	virtual void onEvent(Event& event);

	//Returns true if the element can be selected as a pointer target.
	virtual bool isPointerTarget() const { return false; }
	//Returns true if the point is inside this element hit area.
	virtual bool hitTest(float x, float y) const { return false; }
	//Finds the event target inside this element subtree (default: self only).
	virtual UIElement* findEventTarget(float x, float y);
	bool isMouseInside(float x, float y);
};