#include "Sprite.h"

#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"


Sprite::Sprite(float x, float y, const std::string& id, RenderType type, float width, float height, float rotation, float scale, float zOrder) :
	UIElement(x, y, width, height, rotation, scale, scale, zOrder), m_animator(this)
{
	setTexture(id);
	setType(type);
}

Sprite::~Sprite()
{
	ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_properties.textureID);
}

void Sprite::setTexture(const std::string& textureID)
{
	if (m_properties.textureID == textureID) return;

	ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_properties.textureID);

	m_properties.textureID = textureID;
	// Request texture
	m_properties.texture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(m_properties.textureID);
	resetSourceRegion();
}

void Sprite::setTexture(const std::string& textureID, SDL_FRect sourceRegion)
{
	setSourceRegion(sourceRegion);
	if (m_properties.textureID == textureID) return;
	ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_properties.textureID);

	m_properties.textureID = textureID;
	// Request texture
	m_properties.texture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(m_properties.textureID);
}

void Sprite::setType(RenderType type)
{
	if (type == IMAGE)
	{
		m_properties.imgProp.flipped = false;
	}
	else if(type == NINESLICE)
	{
		m_properties.nslProp.leftW = 0;
		m_properties.nslProp.rightW = 0;
		m_properties.nslProp.topH = 0;
		m_properties.nslProp.botH = 0;
	}

	m_properties.type = type;
}

void Sprite::render(const Mat3f& parentTransform)
{
	// Set color and alpha modulations
	m_properties.texture->setTextureColor(m_properties.rgbaModulation);
	m_properties.texture->setTextureAlpha(m_properties.rgbaModulation.a);

	// Get element global position and rotation
	Mat3f globalTransform = parentTransform * m_transform;
	float rotation = globalTransform.getRotation();
	Vec2f position = globalTransform.getTranslation();

	float finalX = position.getX() - (m_fW * m_scale.getX()) / 2;
	float finalY = position.getY() - (m_fH * m_scale.getY()) / 2;
	float totalWidth = (m_fW * m_scale.getX());
	float totalHeight = (m_fH * m_scale.getY());

	// Render element
	if (m_properties.type == IMAGE)
	{
		m_properties.texture->render(
			{ finalX, finalY, totalWidth, totalHeight }, 
			(m_properties.imgProp.flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE), 
			rotation);

	}
	else if (m_properties.type == NINESLICE)
	{
 		m_properties.texture->render9Grid({ finalX, finalY, totalWidth, totalHeight },
			m_properties.nslProp.leftW, m_properties.nslProp.rightW, m_properties.nslProp.topH, m_properties.nslProp.botH, 1);
	}

	UIElement::render(parentTransform);
}

void Sprite::update(float dt)
{
	m_animator.update(dt);

	UIElement::update(dt);
}
