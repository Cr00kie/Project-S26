#pragma once

#include <array>
#include <memory>
#include <string>

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "BasicResource.h"

constexpr int NUM_CHARACTERS = 255;

// A resource that represents a font loaded from a file path
class Font :
    public BasicResource
{
public:
    // Creates a font which contains all the glyph textures
    // Arguments:
    //		- renderer: the SDL renderer being used
    //		- font: the TTF_Font which we want to use
    Font(SDL_Renderer* renderer, TTF_Font* font);
    ~Font();

	// Render a character to the target rectangle with the given flip mode,
    // rotation angle and color modulation
    void renderCharacter(unsigned char c,
                        const SDL_FRect& targetRect,
                        SDL_FlipMode flipMode = SDL_FLIP_NONE, float rotation = 0, SDL_Color color = {255, 255, 255, 255});

	// Get the width and height of the given text string using the font,
	// it will return true if the text size was successfully obtained, false otherwise
    bool getTextSize(const std::string& text, float& w, float& h) const;

	// Get the SDL texture that represents the glyph for the given character,
	// it will return nullptr if the character is not in the glyph atlas
    inline SDL_Texture* getGlyph(unsigned char c) const {
        return m_GlyphAtlas[c];
    }
    
private:
	// The SDL renderer used to render the font
    SDL_Renderer* m_pRenderer;
	// The SDL_ttf font that represents the font resource
    TTF_Font* m_pFont;
	// An array of SDL textures that represent the glyphs for the characters in the font
    std::array<SDL_Texture*, NUM_CHARACTERS> m_GlyphAtlas;
};

