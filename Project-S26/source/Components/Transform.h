#pragma once
#include "../Math/Vec3.h"
#include "../Math/Mat3.h"

// TODO: add a dirty flag to only update the worldMatrix when necessary
struct Transform
{
	Transform(float x, float y, float rotation = 0.f, float scaleX = 1.f, float scaleY = 1.f)
		: position(x, y), scale(scaleX, scaleY), rotation(rotation) { }

	Transform(Vec2f pos, float rotation = 0.f, Vec2f scale = {1.f, 1.f})
		: position(pos), scale(scale), rotation(rotation) { }

	Transform(Mat3f mat)
		: position(mat.getTranslation()), scale(mat.getScale()), rotation(mat.getRotation()) { }

	Vec2f position = { 0.f, 0.f };
	Vec2f scale =	 { 1.f, 1.f };
	float rotation = 0.f;
	Mat3f worldMatrix = Mat3f(1.f);
};