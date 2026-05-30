#pragma once
#include "../UIElement.h"

#include "../../Resources/Texture.h"
#include <string>
#include <memory>

//Nineslice is a VisualElement that represents a rectangle with a texture that can be stretched without distortion by dividing the texture into 9 parts
//(4 corners, 4 edges and 1 center) and stretching only the edges and the center while keeping the corners unchanged.
//The width of the left and right edges and the height of the top and bottom edges can be set independently,
//so that the corners can be preserved while stretching the edges and the center.
class Nineslice :
	public UIElement
{
private:
	//The texture used to render the nineslice
	Texture* m_pTexture;
	//The ID of the texture used to render the nineslice,
	//used to check if the texture needs to be updated when setTexture is called
	std::string m_sTextureID;

	//The width and height of the nineslice
	float m_fW, m_fH;
	//The width of the left and right edges of the nineslice
	float m_fLeftWidth;
	//The width of the left and right edges of the nineslice
	float m_fRightWidth;
	//The height of the top and bottom edges of the nineslice
	float m_fTopHeight;
	//The height of the top and bottom edges of the nineslice
	float m_fBottomHeight;
	//The scale of the nineslice when rendered
	float m_fScale;
	//The color modulation of the nineslice when rendered
	SDL_Color m_RGBAModulation;

public:
	// Creates an Nineslice
	// Arguments:
	//		- container: the container that will contain the nineslice (scene or another container)
	//		- x: x position of the nineslice
	//		- y: y position of the nineslice
	//		- w: width of the nineslice
	//		- h: height of the nineslice
	//		- id: Asset ID of the texture to be displayed as the nineslice
	//		- leftWidth: size of the left border
	//		- rightWidth: size of the right border
	//		- topHeight: size of the top borders
	//		- bottomHeight: size of the bottom border
	//		- scale: scale of the nineslice (not the normal scale, i also don't understand this)
	Nineslice(float x, float y, float w, float h, const std::string& id, float leftWidth, float rightWidth, float topHeight, float bottomHeight, float scale = 1);
	~Nineslice();

	//Setters and Getters for texture
	//It changes the texture of the nineslice to the texture with the given ID
	void setTexture(const std::string& id);
	//It returns the ID of the texture used to render the nineslice
	const std::string& getTexture() const;

	//Getters
	//It returns the scale of the nineslice when rendered
	inline float getScale() const { return m_fScale; }
	//It returns the width of the nineslice when rendered
	inline float getWidth() const { return m_fW; }
	//It returns the height of the nineslice when rendered
	inline float getHeight() const { return m_fH; }
	//It returns the width of the left edge of the nineslice when rendered
	inline float getLeftWidth() const { return m_fLeftWidth; }
	//It returns the width of the right edge of the nineslice when rendered
	inline float getRightWidth() const { return m_fRightWidth; }
	//It returns the height of the top edge of the nineslice when rendered
	inline float getTopHeight() const { return m_fTopHeight; }
	//It returns the height of the bottom edge of the nineslice when rendered
	inline float getBottomHeight() const { return m_fBottomHeight; }
	//It returns the color modulation of the nineslice when rendered
	inline const SDL_Color& getModulation() const { return m_RGBAModulation; }

	//Setters
	//It changes the scale of the nineslice when rendered
	inline void setScale(float s) { m_fScale = s;}
	//It changes the width of the nineslice when rendered
	inline void setWidth(float w) { m_fW = w; }
	//It changes the height of the nineslice when rendered
	inline void setHeight(float h) { m_fH = h; }
	//It changes the width of the left edge of the nineslice when rendered
	inline void setLeftWidth(float lw) { m_fLeftWidth = lw; }
	//It changes the width of the right edge of the nineslice when rendered
	inline void setRightWidth(float rw) { m_fRightWidth = rw; }
	//It changes the height of the top edge of the nineslice when rendered
	inline void setTopHeight(float th) { m_fTopHeight = th; }
	//It changes the height of the bottom edge of the nineslice when rendered
	inline void setBottomHeight(float bh) { m_fBottomHeight = bh; }
	//It changes the color modulation of the nineslice when rendered
	inline void setModulation(SDL_Color rgba) { m_RGBAModulation = rgba; }
	
	//It renders the nineslice by rendering the 9 parts of the texture
	//with the appropriate scaling and color modulation
	void render(float parentX, float parentY, float parentRot) override;
};

