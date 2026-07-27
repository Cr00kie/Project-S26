#include "TransformHandle.h"

float TransformHandle::getX() const { return get().position.getX(); }

float TransformHandle::getY() const { return get().position.getY(); }

void TransformHandle::setX(float x) { get().position.setX(x); }

void TransformHandle::setY(float y) { get().position.setY(y); }

void TransformHandle::setPosition(float x, float y) { get().position = { x, y }; }

void TransformHandle::setRotation(float rad) { get().rotation = rad; }
