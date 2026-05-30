#pragma once
#include "BasicResource.h"
#include "SDL3/SDL.h"

// A resource that represents a texture loaded from a file path
class Texture :
    public BasicResource
{
public:
    // Creates a Texture resource which wraps SDL_Texture
    // Arguments:
    //		- renderer: the SDL renderer being used
    //      - texture: the texture you want to use
    Texture(SDL_Renderer* renderer, SDL_Texture* texture);
    ~Texture();

	// Delete copy constructor and assignment operator to prevent copying of textures
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

	// Set the color modulation of the texture, it will multiply the texture color by the given color
    inline void setTextureColor(SDL_Color color) { SDL_SetTextureColorMod(m_pTexture, color.r, color.g, color.b);}
	// Set the alpha modulation of the texture, it will multiply the texture alpha by the given alpha value
    inline void setTextureAlpha(int alpha) { SDL_SetTextureAlphaMod(m_pTexture, alpha);}

	// Get the width of the texture
    inline unsigned int getWidth() const { return m_iWidth; }
	// Get the height of the texture
    inline unsigned int getHeight() const { return m_iHeight; }
	// Render the texture to the target rectangle with the given flip mode and rotation angle
    void render(const SDL_FRect& targetRect, SDL_FlipMode flipMode = SDL_FLIP_NONE, double angle = 0);
    void render(const SDL_FRect& sourceRect, const SDL_FRect& targetRect, SDL_FlipMode flipMode = SDL_FLIP_NONE, double angle = 0);
	// Render the texture to the target rectangle with 9-grid scaling,
    void render9Grid(const SDL_FRect& targetRect, float left_width, float right_width, float top_height, float bottom_height, float scale = 1);
    void render9Grid(const SDL_FRect& sourceRect, const SDL_FRect& targetRect, float left_width, float right_width, float top_height, float bottom_height, float scale = 1);
private:
	// The SDL texture that represents the texture resource
    SDL_Texture* m_pTexture = nullptr;
	// The SDL renderer used to render the texture
    SDL_Renderer* m_pRenderer = nullptr;
	// The width and height of the texture
    unsigned int m_iWidth = 0;
    unsigned int m_iHeight = 0;
};

