#include "Rect.h"
#include "../../Tools/ServiceLocator.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../AppFacadeService.h"
#include <SDL3/SDL_render.h>
#include <numbers>
#include <cmath>

void Rect::loadColor(const SDL_Color& color) {
	if (renderer) {
		// Create texture that will represent the rectangle
		SDL_Surface* surface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
		SDL_FillSurfaceRect(surface, NULL, SDL_MapSurfaceRGBA(surface, color.r, color.g, color.b, color.a));
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_DestroySurface(surface);

		m_pTexture = new Texture(renderer, texture);
		setColor(color);
	}
}

Rect::Rect(float x, float y, const SDL_Color& color, float w, float h, bool isFlipped, float rotation, float scale)
	: UIElement(x, y, w, h, rotation), m_bIsFlipped(isFlipped), m_fScaleX(scale), m_fScaleY(scale), m_pTexture(nullptr)
{
	// Get renderer
	auto app = ServiceLocator::tryGet<AppFacadeService>();
	if (app) {
		renderer = &app->getRenderer();
	}
	// Create rect
	loadColor(color);
}

Rect::~Rect(){
	// Release texture
	delete m_pTexture;
	m_pTexture = nullptr;
}

void
Rect::setColor(const SDL_Color& color){
	m_pTexture->setTextureColor(color);
	m_pTexture->setTextureAlpha(color.a);
}

void
Rect::render(const Mat3f& parentTransform) {

	// Get element global position and rotation
	Mat3f globalTransform = parentTransform * m_transform;
	float rotation = globalTransform.getRotation();
	Vec2f position = globalTransform.getTranslation();

	// Render element
	m_pTexture->render({ position.getX() - (m_fW * m_fScaleX) / 2, position.getY() - (m_fH * m_fScaleY) / 2 ,(m_fW * m_fScaleX), (m_fH * m_fScaleY)}, (m_bIsFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE), rotation);

	UIElement::render(parentTransform);
}
