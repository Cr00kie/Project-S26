#pragma once
#include "BasicResource.h"
#include <vector>
#include <string>
#include "SDL3/SDL.h"

struct AnimationFrame
{
	AnimationFrame(const std::string& id, float duration, SDL_FRect region) :
		textureID(id), duration(duration), region(region)
	{
	}
	AnimationFrame(const std::string& id, float duration);
	// Frame data
	float duration;
	std::string textureID;
	SDL_FRect region;
};

class Animation : public BasicResource
{
public:
	Animation() : loop(false) {}
	Animation(const std::vector<AnimationFrame>& frames, bool loops = false) : animationFrames(frames), loop(loops) {}
	std::vector<AnimationFrame> animationFrames;
	bool loop;

	Animation& addFrame(const std::string& texID, float duration)
	{
		animationFrames.emplace_back(texID, duration);
		return *this;
	}
	Animation& addFrame(const std::string& texID, float duration, SDL_FRect region)
	{
		animationFrames.emplace_back(texID, duration, region);
		return *this;
	}
};

