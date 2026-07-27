#include "SpriteRenderableHandle.h"

std::string SpriteRenderableHandle::getTexture() const { return get().textureID; }

void SpriteRenderableHandle::setTexture(const std::string& id) { get().setTexture(id); }

float SpriteRenderableHandle::getWidth() const { return get().width; }

float SpriteRenderableHandle::getHeight() const { return get().height; }

void SpriteRenderableHandle::setWidth(float width) { get().width = width; }

void SpriteRenderableHandle::setHeight(float height) { get().height = height; }
