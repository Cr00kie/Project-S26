#pragma once
#include "SDL3/SDL.h"
#include <string>
#include <vector>

struct AnimationFrame
{
	AnimationFrame(const std::string& id, float duration,  SDL_FRect region) :
		textureID(id), duration(duration), region(region)
	{ }
	AnimationFrame(const std::string& id, float duration);
	// Frame data
	float duration;
	std::string textureID;
	SDL_FRect region;
};

struct Animation
{
	Animation() : loop(false) {}
	Animation(const std::vector<AnimationFrame>& frames, bool loops = false) : animationFrames(frames), loop(loops){}
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

class Sprite;

class SpriteAnimator
{
private:
	Animation* m_animation;
	Sprite* m_sprite;
	std::size_t m_currentFrameIdx;
	float m_ellapsed;
	bool m_playing;

public:
	SpriteAnimator(Sprite* sprite) : m_sprite(sprite), m_animation(nullptr), m_playing(false), m_currentFrameIdx(0), m_ellapsed(0) {}

	SpriteAnimator(Sprite* sprite, Animation* anim, bool startPlaying = true) 
		: m_sprite(sprite), m_animation(anim), m_playing(startPlaying),
		m_currentFrameIdx(0), m_ellapsed(0) {}

	SpriteAnimator(Sprite* sprite, const std::vector<AnimationFrame>& frames, bool startPlaying = true) 
		: SpriteAnimator(sprite, new Animation(frames), startPlaying) {}

	~SpriteAnimator()
	{
		delete m_animation;
	}

	inline void play() { m_playing = true; }
	inline void stop() { m_playing = false; }
	inline bool isPlaying() const { return m_playing; }
	inline void setPlaying(bool playing) { m_playing = playing; }

	void update(float dt);

	inline Animation* getAnimation() const { return m_animation; }
	inline void setAnimation(Animation* animation) { delete m_animation; m_animation = animation; }
	inline void setAnimation(const std::vector<AnimationFrame>& frames, bool loop = false) { delete m_animation;  m_animation = new Animation(frames, loop); }
};

