#pragma once

#include "../Entities/Registry.h"
#include "../Components/SpriteAnimator.h"
#include "../Components/SpriteRenderable.h"

class SpriteAnimationSystem
{
public:
	void update(Registry& reg, float dt)
	{
		for (Entity e : reg.view<SpriteAnimator>())
		{
			if (!reg.has<SpriteRenderable>(e)) continue;

			SpriteRenderable& spr = reg.get<SpriteRenderable>(e);
			SpriteAnimator& antr = reg.get<SpriteAnimator>(e);

			if (!antr.playing || !antr.animation) return;

			antr.ellapsed += dt;

			if (antr.ellapsed > antr.animation->animationFrames[antr.currentFrameIdx].duration)
			{
				antr.currentFrameIdx = (antr.currentFrameIdx + 1) % antr.animation->animationFrames.size();

				if (antr.currentFrameIdx == 0 && !antr.animation->loop)
				{
					antr.stop();
				}
				else
				{
					spr.setTexture(antr.animation->animationFrames[antr.currentFrameIdx].textureID);
				}

				antr.ellapsed = 0;
			}
		}
	}
};