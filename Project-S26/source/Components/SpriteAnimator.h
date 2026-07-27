#pragma once
#include "SDL3/SDL.h"
#include <string>
#include <vector>
#include "../Resources/Animation.h"
#include "../Resources/ResourceManager.h"
#include "../Tools/ServiceLocator.h"

class Sprite;

struct SpriteAnimator
{
	Animation* animation;
	std::string animationID;
	std::size_t currentFrameIdx;
	float ellapsed;
	bool playing;


	SpriteAnimator(const std::string& id, bool startPlaying = true) 
		: currentFrameIdx(0), ellapsed(0), playing(startPlaying)
	{
		setAnimation(id);
	}

	SpriteAnimator(const SpriteAnimator&) = delete;
	SpriteAnimator& operator=(const SpriteAnimator&) = delete;

	SpriteAnimator(SpriteAnimator&& other) noexcept :
		playing(other.playing), currentFrameIdx(other.currentFrameIdx), ellapsed(other.ellapsed)
	{
		animation = other.animation;
		animationID = std::move(other.animationID);

		other.animation = nullptr;
		other.animationID.clear();
	}

	SpriteAnimator& operator=(SpriteAnimator&& other) noexcept
	{
		if (this == &other) return *this;

		animation = other.animation;
		animationID = std::move(other.animationID);

		playing = other.playing;
		currentFrameIdx = other.currentFrameIdx;
		ellapsed = other.ellapsed;

		other.animation = nullptr;
		other.animationID.clear();

		return *this;
	}

	~SpriteAnimator()
	{
		if(!animationID.empty())
			ServiceLocator::get<ResourceManager>().ReleaseResource<Animation>(animationID);
	}

	inline void play() { playing = true; }
	inline void stop() { playing = false; }
	inline bool isPlaying() const { return playing; }
	inline void setPlaying(bool playing) { playing = playing; }

	inline std::string getAnimation() const { return animationID; }
	void setAnimation(const std::string& id);
};

