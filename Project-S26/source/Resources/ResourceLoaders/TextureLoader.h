#pragma once
#include "ResourceLoader.h"
#include "SDL3/SDL.h"

#include <typeindex>

class Texture;

// A resource loader that loads textures from file paths using SDL
class TextureLoader :
    public ResourceLoader
{
public:
    // Creates a TextureLoader to load textures from files
    // Arguments:
    //		- renderer: the SDL renderer being used
    TextureLoader(SDL_Renderer* renderer);

	// Get the type of resource that this loader can load, which is Texture
    std::type_index ResourceType() const override;
	// Load a texture from the given file path and return it as a BasicResource pointer
    BasicResource* Load(const std::string& path) override;
private:
	// The SDL renderer used to create the texture
    SDL_Renderer* m_pRenderer;
};

