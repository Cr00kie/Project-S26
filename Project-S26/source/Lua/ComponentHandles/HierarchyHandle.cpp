#include "HierarchyHandle.h"

Entity HierarchyHandle::getParent() const { return get().parent; }

std::vector<Entity> HierarchyHandle::getChildren() const { return get().children; }
