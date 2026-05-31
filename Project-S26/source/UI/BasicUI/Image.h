#pragma once

#include "../UIElement.h"
#include "../../Resources/Texture.h"
#include "../../Tweens/Tween.h"
#include "../../Math/Mat3.h"

#include <string>
#include <vector>
#include <optional>

// Animation data structure
struct Animation
{
	Animation(){}
	Animation(float fps, unsigned char rows, unsigned char cols, const std::vector<unsigned char> frames, int loops = -1)
	{
		m_fDuration = fps;
		m_rows = rows;
		m_cols = cols;
		m_AnimFrames = frames;
		m_iLoops = loops;
	}
	// Frames per second of the animation
	// FPS == -1 MEANS NO ANIMATION SHOULD BE PLAYED
	float m_fDuration = -1;
	// Number of rows and columns of the spritesheet
	unsigned char m_rows = 1, m_cols = 1;
	// Current frame index
	size_t currFrameIdx = 0;
	// Vector with the frame indices used in the animation
	std::vector<unsigned char> m_AnimFrames;
	//The number of loops the animation should play, -1 means infinite loops
	int m_iLoops = -1;
	// Whether the animation is playing or not
	bool m_bIsPaused = false;
	// The timer used for the animation
	// THIS IS SET AUTOMATICALLY
	Tween* m_timer = nullptr;
};

//Image is a VisualElement that represents a texture rendered as an image in the UI.
//It can be rendered with a rotation, a scale and a color modulation, and it can also be flipped horizontally when rendered.
class Image :
	public UIElement
{
protected:
	//The texture used to render the image
	Texture* m_pTexture;
	//The ID of the texture used to render the image
	std::string m_sTextureID;
	//If the image is flipped horizontally when rendered
	bool m_bIsFlipped;
	//The scale of the image when rendered
	float m_fScale;
	//The width and height of the image when rendered
	float m_fW;
	//The width and height of the image when rendered
	float m_fH;
	//The color modulation of the image when rendered
	SDL_Color m_RGBAModulation;
	//Frame and animation data
	std::optional<Animation> m_Animation;

public:
	// Creates an Image
	// Arguments:
	//		- container: the container that will contain the image (scene or another container)
	//		- x: x position of the image
	//		- y: y position of the image
	//		- id: Asset ID of the texture to be displayed as the image
	//		- scale: scale of the image
	//		- isFlipped: whether to flip the image or not
	//		- rotation: rotation of the image
	Image(float x, float y, const std::string& id, float scale = 1, bool isFlipped = false, float rotation = 0);
	~Image();

	//Getters
	//It returns the ID of the texture used to render the image
	const std::string& getTexture() const;
	//It returns the scale of the image when rendered
	inline float getScale() const { return m_fScale; }
	//It returns the color modulation of the image when rendered
	inline const SDL_Color& getModulation() const { return m_RGBAModulation; }
	//Get animation data
	Animation& getAnimationData() { return m_Animation.value(); }
	//Get whether animation is paused or not
	inline bool getAnimationPause() { return m_Animation.has_value() ? m_Animation->m_bIsPaused : true; }

	//Setters
	//It changes the texture of the image to the texture with the given ID
	void setTexture(const std::string& id);
	//It changes the scale of the image when rendered
	inline void setScale(float s) { m_fScale = s; }
	//It changes the color modulation of the image when rendered
	inline void setModulation(SDL_Color rgba) { m_RGBAModulation = rgba; }
	//Sets animation data
	void setAnimationData(std::optional<Animation> anim);
	//Pause or Resume animation
	void setAnimationPause(bool pause) { 
		if(m_Animation.has_value()) 
			m_Animation->m_bIsPaused = pause; 
	}

	//renders the image
	void render(const Mat3f& parentTransform) override;
};

