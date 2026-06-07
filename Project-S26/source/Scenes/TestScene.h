#pragma once
#include "../UI/UIElement.h"
#include "../UI/Sprite.h"
#include "SceneBuilder.h"
#include "../Tweens/SpriteAnimator.h"
#include "../UI/ParticleEmitter.h"
#include "../UI/Slider.h"
#include "../Debug.h"
#include <iostream>

class TestScene :
    public UIElement
{
public:
    TestScene() : UIElement(0,0)
    {
        Slider* slider = new Slider(500, 500, 0, 500, 50, 0);
        slider->setValue(0.5f);
        slider->addValueChangedListener([this](const float& v)
            {
                DEBUG.DrawText(0, 0, "Value: " + std::to_string(v), 2);
            });
        addChild(slider);
        
    }
private:
};

