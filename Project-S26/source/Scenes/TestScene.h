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
    TestScene() : UIElement(0,0)
    {
        SceneBuilder sb("assets/Tiled/Tilesets/SceneElements.json");
        sb.loadScene(this, "assets/Tiled/Scenes/testScene.json");
        m_cajita = sb.Q<UIElement>("cajita");
        m_padre = sb.Q<UIElement>("Padre");
        m_button = sb.Q<Sprite>("Button");
        elapsed = 0;

        m_emitter = new ParticleEmitter(800, 800, 100, 0.5, true, 25);
        addChild(m_emitter);
        m_emitter->setParticleVelocity(Vec2f(200.f,200.f), Vec2f(-200.f,-200.f));
        m_emitter->setParticleAcceleration(Vec2f(0.f, 50.f));
        m_emitter->setParticleLifetime(1.f, 1.9f);
        m_emitter->setParticleAngularVel(1.f);
        emited = false;
    }
    void update(float dt) override
    {
        UIElement::update(dt);
        elapsed += dt;

        m_button->rotate(dt * 0.2f);
        //m_cajita->setPosition(std::cos(elapsed) * 100.f, m_cajita->getY());
        m_padre->rotate(dt * 0.1f);

        /*if (elapsed > 2)
        {
            m_emitter->emit(100);
            elapsed = 0;
        }*/
    }
private:
    float elapsed;
    UIElement* m_cajita;
    Sprite* m_button;
    UIElement* m_padre;
    ParticleEmitter* m_emitter;
    bool emited;
};

