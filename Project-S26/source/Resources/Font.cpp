#include "Font.h"

#include <cassert>
#include <numbers>

Font::Font(SDL_Renderer* renderer, TTF_Font* font) : m_pRenderer(renderer), m_pFont(font)
{
	// Generate bitmap font from TTF_Font
	for (int i = 0; i < NUM_CHARACTERS; i++)
	{
		// TODO: be more selective about what characters we are creating (most of them return null because the not doesn't do them or smth)
		SDL_Surface* glyphSurface = TTF_RenderGlyph_Blended(m_pFont, (unsigned char) i, { 255,255,255 });
		SDL_Texture* glyphTexture = SDL_CreateTextureFromSurface(renderer, glyphSurface);
		SDL_DestroySurface(glyphSurface);

		m_GlyphAtlas[i] = glyphTexture;
	}
}

Font::~Font()
{
	// Release all created characters
	for (int i = 0; i < NUM_CHARACTERS; i++)
	{
		SDL_DestroyTexture(m_GlyphAtlas[i]);
	}
	TTF_CloseFont(m_pFont);
}

void Font::renderCharacter(unsigned char c, const SDL_FRect& targetRect, SDL_FlipMode flipMode, float rotation, SDL_Color color)
{
	float angle = rotation * 180.f / std::numbers::pi_v<float>;
	// Get character texture
	SDL_Texture* glyphTexture = m_GlyphAtlas[c];

	// Create texture rect
	SDL_FRect sourceRect;
	sourceRect.x = sourceRect.y = 0;
	sourceRect.w = (float)glyphTexture->w; 
	sourceRect.h = (float)glyphTexture->h;

	// Set modulation
	SDL_SetTextureColorMod(glyphTexture, color.r, color.g, color.b);
	SDL_SetTextureAlphaMod(glyphTexture, color.a);

	// Render character
	SDL_RenderTextureRotated(m_pRenderer, glyphTexture, &sourceRect, &targetRect, angle, nullptr, flipMode);
}

bool Font::getTextSize(const std::string& text, float& w, float& h) const
{
	int width, height;
	if (!m_pFont) return false;
	bool ret = TTF_GetStringSize(m_pFont, text.c_str(), text.size(), &width, &height);
	w = float(width);
	h = float(height);
	return ret;
}
