#include "SpriteAnimator.h"
#include "../UI/Sprite.h"
#include "../Resources/Texture.h"

#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"

void SpriteAnimator::update(float dt)
{
	if (!m_playing || !m_animation) return;

	m_ellapsed += dt;

	if (m_ellapsed > m_animation->animationFrames[m_currentFrameIdx].duration)
	{
		m_currentFrameIdx = (m_currentFrameIdx + 1) % m_animation->animationFrames.size();

		if (m_currentFrameIdx == 0 && !m_animation->loop)
		{
			stop();
		}
		else
		{
			m_sprite->setTexture(m_animation->animationFrames[m_currentFrameIdx].textureID);
		}

		m_ellapsed = 0;
	}
}

void SpriteAnimator::setAnimation(const std::string& id)
{
	if (m_animationID == id) return;
	ServiceLocator::get<ResourceManager>().ReleaseResource<Animation>(m_animationID);

	m_animationID = id;
	m_animation = ServiceLocator::get<ResourceManager>().GetResource<Animation>(m_animationID);
}

AnimationFrame::AnimationFrame(const std::string& id, float duration) :
	textureID(id), duration(duration)
{
	Texture* texture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(textureID);
	region = { 0, 0, float(texture->getWidth()), float(texture->getHeight()) };
}
