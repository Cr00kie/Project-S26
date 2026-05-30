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
	//The rotation angle of the rectangle in degrees
	float m_fRotation;
	//The scale of the rectangle in the x and y direction
	float m_fScaleX;
	//The scale of the rectangle in the x and y direction
	float m_fScaleY;
	//The width and height of the rectangle
	float m_fH;
	//The width and height of the rectangle
	float m_fW;

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

	//It changes the rotation of the rectangle when rendered
	inline void setRotation(float r) { m_fRotation = r; }
	//It changes the ScaleX of the rectangle when rendered
	inline void setScaleX(float s) { m_fScaleX = s; }
	//It changes the ScaleY of the rectangle when rendered
	inline void setScaleY(float s) { m_fScaleY = s; }
	//It changes the width of the rectangle when rendered
	inline void setWidth(float w) { m_fW = w; }
	//It changes the height of the rectangle when rendered
	inline void setHeight(float h) { m_fH = h; }
	//It changes whether the rectangle is flipped horizontally when rendered
	inline void setFlipped(bool flipped) { m_bIsFlipped = flipped; }

	//Getters
	//It returns the rotation of the rectangle when rendered
	inline float getRotation() const { return m_fRotation; }
	//It returns the ScaleX of the rectangle when rendered
	inline float getScaleX() const { return m_fScaleX; }
	//It returns the ScaleY of the rectangle when rendered
	inline float getScaleY() const { return m_fScaleY; }
	//It returns the width of the rectangle when rendered
	inline float getWidth() const { return m_fW; }
	//It returns the height of the rectangle when rendered
	inline float getHeight() const { return m_fH; }
	//It returns whether the rectangle is flipped horizontally when rendered
	inline bool isFlipped() const { return m_bIsFlipped; }

	//Render the rectangle
	void render(float parentX, float parentY, float parentRot) override;
};

