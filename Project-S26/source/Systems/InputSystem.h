#pragma once

#include "../Entities/Registry.h"
#include "../Input/InputState.h"
#include "../Components/Transform.h"
#include "../Components/Interactive.h"
#include "../Components/Bounds.h"
#include "../Components/Hierarchy.h"
#include "../Debug.h"

class InputSystem
{
public:
    void update(Registry& reg, const InputState& input)
    {
        Entity target = hitTest(reg, input.mouse.x, input.mouse.y);

        for (Interactive& e : reg.data<Interactive>())
        {
            e.hovered = false;
            e.pressed = false;
        }
        
        // TODO: improve this (this was just to test the ECS and the basic Input system)
        if (target != NullEntity)
        {
            Interactive& interactive = reg.get<Interactive>(target);
            if (input.mouse.left.isPressed()) { 
                interactive.pressed = true; 
                interactive.hovered = false;
            }
            else { 
                interactive.pressed = false;
                interactive.hovered = true; 
            }
        }

        
    }
private:
    Entity hitTest(Registry& reg, float x, float y)
    {
        Entity best = NullEntity;
        float bestZ = -std::numeric_limits<float>::infinity();
        for (Entity e : reg.view<Bounds>())
        {
            if (!reg.has<Interactive>(e)) continue;
            const Mat3f& world = reg.get<Transform>(e).worldMatrix;
            Vec2f local = world.inversed() * Vec2f(x, y);
            const Bounds& b = reg.get<Bounds>(e);
            bool inside = local.getX() >= -b.width / 2 && local.getX() <= b.width / 2
                && local.getY() >= -b.height / 2 && local.getY() <= b.height / 2;
            // Set as target the deepest children
            float z = (float)zOrder(reg, e);
            if (inside && z > bestZ)
            {
                best = e; bestZ = z;
            }
        }
        return best;
    }

    int zOrder(Registry& reg, Entity& e) const
    {
        if (!reg.has<Hierarchy>(e)) return 0;

        Hierarchy& h = reg.get<Hierarchy>(e);

        if (h.parent == NullEntity) return 0;

        return zOrder(reg, h.parent) + 1;
    }
};