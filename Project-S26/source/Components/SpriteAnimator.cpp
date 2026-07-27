#include "SpriteAnimator.h"
#include "../Resources/Texture.h"

#include "../Tools/ServiceLocator.h"
#include "../Resources/ResourceManager.h"

void SpriteAnimator::setAnimation(const std::string& id)
{
	if (animationID == id) return;

	if(!animationID.empty())
		ServiceLocator::get<ResourceManager>().ReleaseResource<Animation>(animationID);

	animationID = id;
	animation = ServiceLocator::get<ResourceManager>().GetResource<Animation>(animationID);
}
