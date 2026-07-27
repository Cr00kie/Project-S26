#pragma once
#include <string>
#include "SDL3/SDL.h"
#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"

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

struct SpriteRenderable
{
    SpriteRenderable(RenderType type, std::string textureID,
        float width = -1.f, float height = -1.f,
        SDL_Color mod = { 255, 255, 255, 255 },
        SDL_FRect source = { -1.f, -1.f, -1.f, -1.f }) :
        type(type), width(width), height(height), rgbaModulation(mod), sourceRegion(source)
    {
        setTexture(textureID);
    }

    SpriteRenderable(SpriteRenderable&& other) noexcept
        : type(other.type), texture(other.texture), textureID(std::move(other.textureID)),
        rgbaModulation(other.rgbaModulation), sourceRegion(other.sourceRegion),
        width(other.width), height(other.height)
    {
        if (type == IMAGE) imgProp = other.imgProp;
        else                nslProp = other.nslProp;

        other.texture = nullptr;
        other.textureID.clear();
    }

    SpriteRenderable& operator=(SpriteRenderable&& other) noexcept
    {
        if (this == &other) return *this;

        // Release whatever THIS object currently owns before taking
        // ownership of other's resource.
        if (!textureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(textureID);

        type = other.type;
        texture = other.texture;
        textureID = std::move(other.textureID);
        rgbaModulation = other.rgbaModulation;
        sourceRegion = other.sourceRegion;
        width = other.width;
        height = other.height;
        if (type == IMAGE) imgProp = other.imgProp;
        else                nslProp = other.nslProp;

        other.texture = nullptr;
        other.textureID.clear();
        return *this;
    }

    ~SpriteRenderable()
    {
        if (!textureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(textureID);
    }

    SpriteRenderable(const SpriteRenderable&) = delete;
    SpriteRenderable& operator=(const SpriteRenderable&) = delete;

    RenderType type = RenderType::IMAGE;
    Texture* texture = nullptr;
    std::string textureID;
    SDL_Color rgbaModulation = { 255, 255, 255, 255 };
    SDL_FRect sourceRegion = {-1, -1, -1, -1};
    float width = -1.f; float height = -1.f;
    union
    {
        ImageProperties imgProp;
        NinesliceProperties nslProp;
    };


    void setTexture(const std::string& texID)
    {
        if (texID == textureID) return;

        if (!textureID.empty())
            ServiceLocator::get<ResourceManager>().ReleaseResource<Texture>(textureID);

        textureID = texID;
        // Request texture
        texture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(textureID);
    }
};