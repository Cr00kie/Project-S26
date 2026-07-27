#pragma once

#include "../Entities/Registry.h"
#include "../Components/Transform.h"
#include "../Components/SpriteRenderable.h"
#include "../Resources/Texture.h"

class SpriteRenderSystem
{
public:
	void render(Registry& reg)
	{
		for (Entity e : reg.view<SpriteRenderable>())
		{
			const SpriteRenderable& sr = reg.get<SpriteRenderable>(e);
			const Transform& tr = reg.get<Transform>(e);

			Vec2f worldPos = tr.worldMatrix.getTranslation();
			float worldRot = tr.worldMatrix.getRotation();
			Vec2f scale = tr.scale;

			// Set color and alpha modulations
			sr.texture->setTextureColor(sr.rgbaModulation);
			sr.texture->setTextureAlpha(sr.rgbaModulation.a);

			// Get target region from object position
			float width = sr.width == -1 ? float(sr.texture->getWidth()) : sr.width;
			float height = sr.height == -1 ? float(sr.texture->getHeight()) : sr.height;

			float finalX = worldPos.getX() - (width * scale.getX()) / 2;
			float finalY = worldPos.getY() - (height * scale.getY()) / 2;
			float totalWidth = (width * scale.getX());
			float totalHeight = (height * scale.getY());

			SDL_FRect target = { finalX, finalY, totalWidth, totalHeight };

			// Get source region
			SDL_FRect source = sr.sourceRegion;
			// If no source region was specified, use the texture size by default
			if (sr.sourceRegion.x == -1)
			{
				source = { 0.f, 0.f, (float)sr.texture->getWidth(), (float)sr.texture->getHeight() };
			}

			// Render element
			if (sr.type == IMAGE)
			{
				sr.texture->render(source, target,
					(sr.imgProp.flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE),
					worldRot);

			}
			else if (sr.type == NINESLICE)
			{
				sr.texture->render9Grid(source,target,
					sr.nslProp.leftW, sr.nslProp.rightW, sr.nslProp.topH, sr.nslProp.botH, 1.f);
			}
		}
	}
};