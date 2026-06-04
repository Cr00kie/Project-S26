#pragma once
#include "../UI/UIElement.h"
#include "../UI/BasicUI/Sprite.h"
#include "SceneBuilder.h"
#include <iostream>

class TestScene :
    public UIElement
{
public:
    TestScene() : UIElement(0,0)
    {
        SceneBuilder sb("assets/Tiled/Tilesets/SceneElements.json");
        sb.loadScene(this, "assets/Tiled/Scenes/testScene.json");
        m_text = sb.Q<UIElement>("text");
        m_button = sb.Q<UIElement>("Button");


    }
    void update(float dt) override
    {
        UIElement::update(dt);

        m_button->rotate(dt * 0.2f);
    }
private:
    UIElement* m_text;
    UIElement* m_button;
};

