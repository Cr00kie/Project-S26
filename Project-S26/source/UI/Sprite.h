#pragma once
#include "UIElement.h"

#include <cassert>

#include "../Resources/Texture.h"
#include "../Tweens/SpriteAnimator.h"

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
    SDL_FRect sourceRegion;
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
    SpriteAnimator m_animator;

public:
    Sprite(float x, float y, const std::string& id, RenderType type = IMAGE, float width = 0, float height = 0, float rotation = 0, float scale = 1, float zOrder = 0);
    ~Sprite();

    void setTexture(const std::string& textureID);
    void setTexture(const std::string& textureID, SDL_FRect sourceRegion);
    inline const std::string& getTexture() const { return m_properties.textureID; }

    inline void setSourceRegion(SDL_FRect sourceRegion) { m_properties.sourceRegion = sourceRegion; }
    inline void resetSourceRegion() { m_properties.sourceRegion = { 0, 0, (float)m_properties.texture->getWidth(), (float)m_properties.texture->getHeight() }; }

    void setRGBAModulation(SDL_Color modulation) { m_properties.rgbaModulation = modulation; }
    void setOpacity(Uint8 alpha) { m_properties.rgbaModulation.a = alpha; }
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
    inline void setBorderSize(float lw, float rw, float th, float bh)
    {
        assert(m_properties.type == NINESLICE);
        m_properties.nslProp.leftW = lw;
        m_properties.nslProp.rightW = rw;
        m_properties.nslProp.topH = th;
        m_properties.nslProp.botH = bh;
    }

    SpriteAnimator& getAnimator() { return m_animator; }

    void render(const Mat3f& parentTransform) override;
    void update(float dt) override;
};

