#include "Image.h"
#include "../../Tools/ServiceLocator.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include <numbers>
#include <cmath>
#include <iostream>

Image::Image(float x, float y, const std::string& id, float scale, bool isFlipped, float rotation)
	: UIElement(x, y, 0, 0, rotation, scale, scale), m_bIsFlipped(isFlipped), m_RGBAModulation({ 255,255,255,255 })
{
	setTexture(id);
}

Image::~Image()
{
	if (m_Animation.has_value())
	{
		// Delete animation tween timer
		//m_Animation->m_timer->finish();
		m_Animation->m_timer = nullptr;
	}
	// Release Texture
	ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_sTextureID);
}

void
Image::setTexture(const std::string& id) {
	m_sTextureID = id;
	// Request texture
	m_pTexture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(m_sTextureID);

	m_fW = float(m_pTexture->getWidth());
	m_fH = float(m_pTexture->getHeight());
}

const std::string& Image::getTexture() const
{
	return m_sTextureID;
}


//Sets animation data
void Image::setAnimationData(std::optional<Animation> anim) {
	if (m_Animation.has_value())
	{
		// remove previous timer
		if (m_Animation->m_timer) {
			m_Animation->m_timer->finish();
		}
		m_Animation->m_timer = nullptr;
	}

	m_Animation = anim;
	if (!m_Animation.has_value()) return;

	float cycleDuration = m_Animation->m_fDuration * m_Animation->m_AnimFrames.size() / 2;
	int tweenLoops = (m_Animation->m_iLoops == -1) ? -1 : (m_Animation->m_iLoops - 1);

	m_Animation->m_timer = Tween::makeTween(
		0,
		1000,
		cycleDuration,
		[this](float dt)
		{
			if (!m_Animation->m_bIsPaused) {
				unsigned int totalFrames = int(m_Animation->m_AnimFrames.size());
				unsigned int frame = int(floor((dt / 1000.0f) * totalFrames));
				frame = std::clamp(frame, 0u, totalFrames - 1);
				m_Animation->currFrameIdx = frame;
			}
		},
		tweenLoops
	);

	m_Animation->m_timer->onComplete([this]() {
		m_Animation->m_timer = nullptr;
		});
}

void
Image::render(const Mat3f& parentTransform) {
	// Set color and alpha modulations
	m_pTexture->setTextureColor(m_RGBAModulation);
	m_pTexture->setTextureAlpha(m_RGBAModulation.a);

	// Get element global position and rotation
	Mat3f globalTransform = parentTransform * m_transform;
	float rotation = globalTransform.getRotation();
	Vec2f position = globalTransform.getTranslation();

	// Render element
	if (!m_Animation.has_value())
	{
		m_pTexture->render(
			{ position.getX() - (m_fW * m_scale.getX()) / 2, position.getY() - (m_fH * m_scale.getY()) / 2 ,(m_fW * m_scale.getX()), (m_fH * m_scale.getY())},
			(m_bIsFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE),
			rotation
		);
	}
	else
	{
		Animation& anim = m_Animation.value();

		unsigned char frame = anim.m_AnimFrames[anim.currFrameIdx];
		float frameWidth = m_fW / float(anim.m_cols);
		float frameHeight = m_fH / float(anim.m_rows);

		SDL_FRect sourceRect = {
			(frame % anim.m_cols) * frameWidth,
			(frame / anim.m_cols) * frameHeight,
			frameWidth,
			frameHeight,
		};

		m_pTexture->render(
			sourceRect,
			{ position.getX() - (frameWidth * m_scale.getX()) / 2, position.getY() - (frameHeight * m_scale.getY()) / 2 ,(frameWidth * m_scale.getX()), (frameHeight * m_scale.getY())},
			(m_bIsFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE),
			rotation
		);
	}

	UIElement::render(parentTransform);
}
