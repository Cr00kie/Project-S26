#pragma once

#include "BaseComponentHandle.h"
#include "../../Entities/Registry.h"
#include "../../Components/SpriteAnimator.h"

class SpriteAnimatorHandle : public BaseComponentHandle<SpriteAnimator>
{
	LUA_EXPOSE_TYPE(SpriteAnimatorHandle, SpriteAnimatorHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	SpriteAnimatorHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e) {}

	void play();
	void stop();
	bool isPlaying() const;
	void setPlaying(bool playing);
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, play, play)
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, stop, stop)
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, isPlaying, is_playing)
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, setPlaying, set_playing)

	std::string getAnimation() const { return get().getAnimation(); }
	void setAnimation(const std::string& id);
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, getAnimation, get_animation)
	LUA_EXPOSE_MEMBER(SpriteAnimatorHandle, setAnimation, set_animation)

};
LUA_EXPOSE_COMPONENT_GETTER(SpriteAnimatorHandle, get_animator)