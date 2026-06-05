#pragma once
#include "../UI/UIElement.h"
#include "../UI/BasicUI/Sprite.h"
#include "SceneBuilder.h"
#include "../Tweens/SpriteAnimator.h"
#include <iostream>

class TestScene :
    public UIElement
{
public:
    TestScene() : UIElement(0,0)
    {
        SceneBuilder sb("assets/Tiled/Tilesets/SceneElements.json");
        sb.loadScene(this, "assets/Tiled/Scenes/testScene.json");
        m_cajita = sb.Q<UIElement>("cajita");
        m_padre = sb.Q<UIElement>("Padre");
        m_button = sb.Q<Sprite>("Button");
        elapsed = 0;

        SpriteAnimator& sa = m_button->getAnimator();
        sa.setAnimation("buttonAnim");
        sa.play();
    }
    void update(float dt) override
    {
        UIElement::update(dt);
        elapsed += dt;

        m_button->rotate(dt * 0.2f);
        m_cajita->setPosition(std::cos(elapsed) * 100.f, m_cajita->getY());
        m_padre->rotate(dt * 0.2f);
    }
private:
    float elapsed;
    UIElement* m_cajita;
    Sprite* m_button;
    UIElement* m_padre;
};

