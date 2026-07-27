#pragma once
#include "../Entities/Entity.h"
#include <vector>

struct Hierarchy
{
	Entity parent = NullEntity;
	std::vector<Entity> children;
};