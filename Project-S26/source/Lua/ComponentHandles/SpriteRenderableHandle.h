#pragma once

#include "../LuaBindingTools.h"
#include "BaseComponentHandle.h"
#include "../../Components/SpriteRenderable.h"

class SpriteRenderableHandle : public BaseComponentHandle<SpriteRenderable>
{
	LUA_EXPOSE_TYPE(SpriteRenderableHandle, SpriteRenderableHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	SpriteRenderableHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e) {}

	std::string getTexture() const;
	void setTexture(const std::string& id);
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, getTexture, get_texture)
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, setTexture, set_texture)

	float getWidth() const;
	float getHeight() const;
	void setWidth(float width);
	void setHeight(float height);
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, getWidth, width)
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, getHeight, height)
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, setWidth, set_width)
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, setHeight, set_height)

	void setColorModulation(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { get().rgbaModulation = { r, g, b, a }; }
	LUA_EXPOSE_MEMBER(SpriteRenderableHandle, setColorModulation, set_color)


};
LUA_EXPOSE_COMPONENT_GETTER(SpriteRenderableHandle, get_sprite)