#include "Animation.h"
#include "Texture.h"
#include "../Tools/ServiceLocator.h"
#include "ResourceManager.h"

AnimationFrame::AnimationFrame(const std::string& id, float duration) :
	textureID(id), duration(duration)
{
	Texture* texture = ServiceLocator::get<ResourceManager>().GetResource<Texture>(textureID);
	region = { 0, 0, float(texture->getWidth()), float(texture->getHeight()) };
}