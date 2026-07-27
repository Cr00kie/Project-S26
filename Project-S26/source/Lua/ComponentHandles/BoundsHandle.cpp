#include "BoundsHandle.h"

inline float BoundsHandle::getWidth() const { return get().width; }

inline float BoundsHandle::getHeight() const { return get().height; }

inline void BoundsHandle::setWidth(float w) { get().width = w; }

inline void BoundsHandle::setHeight(float h) { get().height = h; }
