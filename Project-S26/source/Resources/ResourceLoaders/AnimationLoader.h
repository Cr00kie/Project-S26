#pragma once
#include "ResourceLoader.h"

class AnimationLoader :
    public ResourceLoader
{
    // Inherited via ResourceLoader
    BasicResource* Load(const std::string& path) override;

    // Inherited via ResourceLoader
    std::type_index ResourceType() const override;
};

