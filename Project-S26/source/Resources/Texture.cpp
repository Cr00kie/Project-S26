#include "Texture.h"
#include "../SDLApplication.h"

Texture::Texture(SDL_Renderer* renderer, SDL_Texture* texture) : m_pRenderer(renderer), m_pTexture(texture), m_iWidth(texture->w), m_iHeight(texture->h)
{
	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
}

Texture::~Texture()
{
	SDL_DestroyTexture(m_pTexture);
}

void Texture::render(const SDL_FRect& targetRect, SDL_FlipMode flipMode, double angle)
{
	SDL_FRect sourceRect (0, 0, (float)m_iWidth, (float)m_iHeight);
	SDL_RenderTextureRotated(m_pRenderer, m_pTexture, &sourceRect, &targetRect, angle, nullptr, flipMode);
}

void Texture::render9Grid(const SDL_FRect& targetRect, float left_width, float right_width, float top_height, float bottom_height, float scale)
{
	SDL_FRect sourceRect(0, 0, (float)m_iWidth, (float)m_iHeight);
	SDL_RenderTexture9Grid(m_pRenderer, m_pTexture, &sourceRect, left_width, right_width, top_height, bottom_height, scale, &targetRect);
}

void Texture::render(const SDL_FRect& sourceRect, const SDL_FRect& targetRect, SDL_FlipMode flipMode, double angle)
{
	SDL_RenderTextureRotated(m_pRenderer, m_pTexture, &sourceRect, &targetRect, angle, nullptr, flipMode);
}

void Texture::render9Grid(const SDL_FRect& sourceRect, const SDL_FRect& targetRect, float left_width, float right_width, float top_height, float bottom_height, float scale)
{
	SDL_RenderTexture9Grid(m_pRenderer, m_pTexture, &sourceRect, left_width, right_width, top_height, bottom_height, scale, &targetRect);
}
