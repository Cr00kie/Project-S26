#include "FontLoader.h"

#include <cassert>
#include <memory>

#include "SDL3_ttf/SDL_ttf.h"

#include "../../Resources/Font.h"

FontLoader::FontLoader(SDL_Renderer* renderer, float fontSize) : m_pRenderer(renderer), m_fFontSize(fontSize)
{
}

BasicResource* FontLoader::Load(const std::string& path)
{
	TTF_Font* font = TTF_OpenFont(path.c_str(), m_fFontSize);

	assert(font != nullptr);

	return new Font(m_pRenderer, font);
}

std::type_index FontLoader::ResourceType() const
{
	return typeid(Font);
}
