#pragma once

#include "../Lua/LuaBindingTools.h"

class TransformHandle;

#define INVALID_ID std::numeric_limits<unsigned int>::max()

struct Entity
{
    LUA_EXPOSE_TYPE(Entity, Entity, CONSTRUCTORS(Entity()), NO_BASE)
    
    unsigned int id = INVALID_ID;
    LUA_EXPOSE_MEMBER(Entity, id, id)

    unsigned int generation = INVALID_ID;

    bool operator==(const Entity& o) const { return id == o.id && generation == o.generation; }
    bool operator!=(const Entity& o) const { return !(*this == o); }
};
constexpr Entity NullEntity{ INVALID_ID, INVALID_ID };