#pragma once
#include "ResourceLoader.h"

#include "SDL3/SDL.h"
#include <typeindex>

// A resource loader that loads fonts from file paths using SDL_ttf
class FontLoader :
    public ResourceLoader
{
public:
    // Creates a FontLoader to load fonts from files
    // Arguments:
    //		- renderer: the SDL renderer being used
    //		- fontSize: the size of the fonts this loader will create
    FontLoader(SDL_Renderer* renderer, float fontSize);

	// Load a font from the given file path and return it as a BasicResource pointer
    BasicResource* Load(const std::string& path) override;
	// Get the type of resource that this loader can load, which is Font
    std::type_index ResourceType() const override;
private:
	// The SDL renderer used to create the font
    SDL_Renderer* m_pRenderer;
	// The font size used to create the font
    float m_fFontSize;
};

