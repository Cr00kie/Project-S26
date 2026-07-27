#pragma once
#include "BaseComponentHandle.h"
#include "../../Components/Transform.h"

class TransformHandle : public BaseComponentHandle<Transform>
{
public:
	LUA_EXPOSE_TYPE(TransformHandle, Transform, NO_CONSTRUCTOR, NO_BASE)

	TransformHandle(Registry& reg, Entity e) : 
		BaseComponentHandle(reg, e){}

	float getX() const;
	float getY() const;
	void setX(float x);
	void setY(float y);
	void setPosition(float x, float y);
	LUA_EXPOSE_MEMBER(TransformHandle, getX, get_x)
	LUA_EXPOSE_MEMBER(TransformHandle, getY, get_y)
	LUA_EXPOSE_MEMBER(TransformHandle, setX, set_x)
	LUA_EXPOSE_MEMBER(TransformHandle, setY, set_y)
	LUA_EXPOSE_MEMBER(TransformHandle, setPosition, set_position)

	float getRotation() const { return get().rotation; }
	void setRotation(float rad);
	LUA_EXPOSE_MEMBER(TransformHandle, getRotation, get_rotation)
	LUA_EXPOSE_MEMBER(TransformHandle, setRotation, set_rotation)
};
LUA_EXPOSE_COMPONENT_GETTER(TransformHandle, get_transform)