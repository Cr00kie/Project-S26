#include "AnimationLoader.h"
#include "../Animation.h"
#include <json.hpp>
#include <fstream>

std::type_index AnimationLoader::ResourceType() const
{
	return typeid(Animation);
}

BasicResource* AnimationLoader::Load(const std::string& path)
{
	std::ifstream file(path);
	if (!file.is_open())
		throw std::runtime_error("Could not open json animation file: " + path);

	nlohmann::json animationJSON;
	file >> animationJSON;

	Animation* anim = new Animation();
	auto& frames = animationJSON["frames"];
	for (auto& frameJSON : frames)
	{
		auto& regionJSON = frameJSON["srcRect"];
		if (regionJSON.is_null())
		{
			anim->addFrame(frameJSON["srcID"], frameJSON["duration"]);
		}
		else
		{
			anim->addFrame(frameJSON["srcID"], frameJSON["duration"], { regionJSON["x"], regionJSON["y"], regionJSON["w"], regionJSON["h"] });
		}
			
	}
	anim->loop = animationJSON["loop"];

	return anim;
}
