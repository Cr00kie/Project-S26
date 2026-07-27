#pragma once
#include "../LuaBindingTools.h"
#include "BaseComponentHandle.h"
#include "../../Components/Hierarchy.h"

class HierarchyHandle : public BaseComponentHandle<Hierarchy>
{
	LUA_EXPOSE_TYPE(HierarchyHandle, HierarchyHandle, NO_CONSTRUCTOR, NO_BASE)
public:
	HierarchyHandle(Registry& reg, Entity e) : BaseComponentHandle(reg, e) {}

	Entity getParent() const;
	LUA_EXPOSE_MEMBER(HierarchyHandle, getParent, parent)

	std::vector<Entity> getChildren() const;
	LUA_EXPOSE_MEMBER(HierarchyHandle, getChildren, children)
};
LUA_EXPOSE_COMPONENT_GETTER(HierarchyHandle, get_hierarchy)