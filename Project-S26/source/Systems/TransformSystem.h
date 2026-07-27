#pragma once
#include "../Entities/Registry.h"
#include "../Math/Mat3.h"

#include "../Components/Transform.h"
#include "../Components/Hierarchy.h"

class TransformSystem{
public:
    void update(Registry& reg)
    {
        for (Entity e : reg.view<Transform>())
        {
            bool isRoot = !reg.has<Hierarchy>(e) || reg.get<Hierarchy>(e).parent == NullEntity;
            if (isRoot) propagate(reg, e, Mat3f(1.f));
        }
    }
private:
    void propagate(Registry& reg, Entity e, const Mat3f& parentWorld)
    {
        Transform& t = reg.get<Transform>(e);
        // Recompute matrix to avoid calculation errors
        t.worldMatrix = parentWorld * Mat3f::translation(t.position) * Mat3f::rotation(t.rotation) * Mat3f::scale(t.scale) ;
        if (reg.has<Hierarchy>(e))
            for (Entity child : reg.get<Hierarchy>(e).children)
                propagate(reg, child, t.worldMatrix);
    }
};