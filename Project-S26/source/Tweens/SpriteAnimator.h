#pragma once
#include "SDL3/SDL.h"
#include <string>
#include <vector>
#include "../Resources/Animation.h"
#include "../Resources/ResourceManager.h"
#include "../Tools/ServiceLocator.h"

class Sprite;

class SpriteAnimator
{
private:
	Animation* m_animation;
	std::string m_animationID;
	Sprite* m_sprite;
	std::size_t m_currentFrameIdx;
	float m_ellapsed;
	bool m_playing;

public:
	SpriteAnimator(Sprite* sprite) : m_sprite(sprite), m_animation(nullptr), m_playing(false), m_currentFrameIdx(0), m_ellapsed(0) {}

	SpriteAnimator(Sprite* sprite, const std::string& id, bool startPlaying = true) 
		: m_sprite(sprite), m_animationID(id), m_playing(startPlaying),
		m_currentFrameIdx(0), m_ellapsed(0) {
		setAnimation(id);
	}

	~SpriteAnimator()
	{
		ServiceLocator::get<ResourceManager>().ReleaseResource<Animation>(m_animationID);
	}

	inline void play() { m_playing = true; }
	inline void stop() { m_playing = false; }
	inline bool isPlaying() const { return m_playing; }
	inline void setPlaying(bool playing) { m_playing = playing; }

	void update(float dt);

	inline std::string getAnimation() const { return m_animationID; }
	void setAnimation(const std::string& id);
};

