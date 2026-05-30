#include "TextureLoader.h"
#include "SDL3_image/SDL_image.h"
#include "../../Resources/Texture.h"

#include <cassert>
#include <memory>

TextureLoader::TextureLoader(SDL_Renderer* renderer) : m_pRenderer(renderer){}

BasicResource* TextureLoader::Load(const std::string& path)
{
    SDL_Texture* sdlTexture = IMG_LoadTexture(m_pRenderer, path.c_str());

    assert(sdlTexture != nullptr);

    return new Texture(m_pRenderer, sdlTexture);
}

std::type_index TextureLoader::ResourceType() const
{
    return typeid(Texture);
}
