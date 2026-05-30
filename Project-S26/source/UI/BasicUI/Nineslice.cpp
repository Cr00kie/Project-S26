#include "Nineslice.h"

#include "../../Resources/ResourceManager.h"
#include "../../Tools/ServiceLocator.h"
#include "../../Resources/Texture.h"

Nineslice::Nineslice(float x, float y, float w, float h, const std::string& id, float leftWidth, float rightWidth, float topHeight, float bottomHeight, float scale)
	:
	UIElement(x, y),
	m_fW(w), m_fH(h),
	m_fLeftWidth(leftWidth), m_fRightWidth(rightWidth),
	m_fTopHeight(topHeight), m_fBottomHeight(bottomHeight),
	m_fScale(scale),
	m_RGBAModulation({ 255,255,255,255 })
{
	setTexture(id);
}

Nineslice::~Nineslice()
{
	// Release Texture
	ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(m_sTextureID);
}

void
Nineslice::setTexture(const std::string& id)
{
	m_sTextureID = id;
	// Request texture
	m_pTexture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(m_sTextureID);
}

const std::string& Nineslice::getTexture() const
{
	return m_sTextureID;
}

void
Nineslice::render(float parentX, float parentY, float parentRot) {
	// Set color and alpha modulations
	m_pTexture->setTextureColor(m_RGBAModulation);
	m_pTexture->setTextureAlpha(m_RGBAModulation.a);

	// Get element global position
	float x = m_fX + parentX;
	float y = m_fY + parentY;

	// Render element as nineslice
	m_pTexture->render9Grid({ x - m_fW / 2, y - m_fH / 2, m_fW, m_fH },
		m_fLeftWidth, m_fRightWidth, m_fTopHeight, m_fBottomHeight, m_fScale);
}
