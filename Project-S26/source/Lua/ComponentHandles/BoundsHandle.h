#pragma once

#include "../LuaBindingTools.h"
#include "BaseComponentHandle.h"
#include "../../Components/Bounds.h"

class BoundsHandle : public BaseComponentHandle<Bounds>
{
	LUA_EXPOSE_TYPE(BoundsHandle, BoundsHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	BoundsHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e){}

	float getWidth() const;
	float getHeight() const;
	void setWidth(float w);
	void setHeight(float h);
	LUA_EXPOSE_MEMBER(BoundsHandle, getWidth, width)
	LUA_EXPOSE_MEMBER(BoundsHandle, getHeight, height)
	LUA_EXPOSE_MEMBER(BoundsHandle, setWidth, set_width)
	LUA_EXPOSE_MEMBER(BoundsHandle, setHeight, set_height)
};
LUA_EXPOSE_COMPONENT_GETTER(BoundsHandle, get_bounds)