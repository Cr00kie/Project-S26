#pragma once
#include "../UIElement.h"

#include <cassert>

#include "../../Resources/Texture.h"

enum RenderType
{
    IMAGE,
    NINESLICE
};

struct ImageProperties
{
    bool flipped = false;
};

struct NinesliceProperties
{
    float leftW = 0;
    float rightW = 0;
    float topH = 0;
    float botH = 0;
};

struct SpriteProperties
{
    SpriteProperties() {}
    RenderType type;
    Texture* texture;
    std::string textureID;
    SDL_Color rgbaModulation = { 255, 255, 255, 255 };
    union
    {
        ImageProperties imgProp;
        NinesliceProperties nslProp;
    };
};

class Sprite :
    public UIElement
{
private:
    SpriteProperties m_properties;

public:
    Sprite(float x, float y, const std::string& id, RenderType type = IMAGE, float width = 0, float height = 0, float rotation = 0, float scale = 1, float zOrder = 0);
    ~Sprite();

    void setTexture(const std::string& textureID);
    inline const std::string& getTexture() const { return m_properties.textureID; }

    void setRGBAModulation(SDL_Color modulation) { m_properties.rgbaModulation = modulation; }
    SDL_Color getRGBAModulation() const { return m_properties.rgbaModulation; }

    void setType(RenderType type);
    RenderType getType() const { return m_properties.type; }

    inline void setFlipped(bool flipped) { assert(m_properties.type == IMAGE); m_properties.imgProp.flipped = flipped; }
    inline bool getFlipped() const { assert(m_properties.type == IMAGE); return m_properties.imgProp.flipped; }

    inline void setLeftWidth(float lw) { assert(m_properties.type == NINESLICE); m_properties.nslProp.leftW = lw; }
    inline float getLeftWidth() const { assert(m_properties.type == NINESLICE); return m_properties.nslProp.leftW; }

    inline void setRightWidth(float rw) { assert(m_properties.type == NINESLICE); m_properties.nslProp.rightW = rw; }
    inline float getRightWidth() const { assert(m_properties.type == NINESLICE); return m_properties.nslProp.rightW; }

    inline void setTopHeight(float th) { assert(m_properties.type == NINESLICE); m_properties.nslProp.topH = th; }
    inline float getTopHeight() const { assert(m_properties.type == NINESLICE); return m_properties.nslProp.topH; }

    inline void setBottomHeight(float bh) { assert(m_properties.type == NINESLICE); m_properties.nslProp.botH = bh; }
    inline float getBottomHeight() const { assert(m_properties.type == NINESLICE); return m_properties.nslProp.botH; }

    inline void setBorderSize(float size) { assert(m_properties.type == NINESLICE); 
                                        m_properties.nslProp.leftW = m_properties.nslProp.rightW = m_properties.nslProp.topH = m_properties.nslProp.botH = size; }

    void render(const Mat3f& parentTransform) override;
};

