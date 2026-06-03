#pragma once

#include "../UIElement.h"
#include "../../Resources/Texture.h"

#include <string>
#include <memory>

class Texture;

//Rect is a VisualElement that represents a rectangle with a solid color.
//It can be rendered with a rotation, a scale and a color modulation, and it can also be flipped horizontally when rendered.
class Rect :
	public UIElement
{
private:
	//The renderer used to render the rectangle
	SDL_Renderer* renderer;
	//The texture used to render the rectangle
	Texture* m_pTexture;
	//If the rectangle is flipped horizontally when rendered
	bool m_bIsFlipped;

	//It loads the texture of the rectangle with the given color
	void loadColor(const SDL_Color& color);
public:
	// Creates an Rect
	// Arguments:
	//		- container: the container that will contain the rect (scene or another container)
	//		- x: x position of the rect
	//		- y: y position of the rect
	//		- color: color of the rect
	//		- w: width of the rect
	//		- h: height of the rect
	//		- isFlipped: whether to flip the rect or not
	//		- rotation: rotation of the rect
	//		- scale: scale of the rect
	Rect(float x, float y, const SDL_Color& color, float w, float h, bool isFlipped = false, float rotation = 0, float scale = 1);

	virtual ~Rect();
	//Setters
	//It changes the color of the rectangle by loading a new texture with the given color
	void setColor(const SDL_Color& color);

	//It changes whether the rectangle is flipped horizontally when rendered
	inline void setFlipped(bool flipped) { m_bIsFlipped = flipped; }

	//Getters
	//It returns whether the rectangle is flipped horizontally when rendered
	inline bool isFlipped() const { return m_bIsFlipped; }

	//Render the rectangle
	void render(const Mat3f& parentTransform) override;
};

