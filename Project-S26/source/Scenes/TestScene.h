#pragma once
#include "../UI/UIElement.h"
#include "../UI/BasicUI/Sprite.h"
#include "SceneBuilder.h"
#include "../Tweens/SpriteAnimator.h"
#include "../UI/ParticleEmitter.h"
#include <iostream>

class TestScene :
    public UIElement
{
public:
    TestScene(float x, float y, float w, float h, SDL_Color color) : UIElement(0,0)
    {
        Sprite* sprite = new Sprite(x, y, "square", IMAGE, w, h);
        addChild(sprite);
        sprite->setRGBAModulation(color);
    }
private:
};

