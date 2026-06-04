#pragma once
#include "../UI/UIElement.h"
#include "../UI/BasicUI/Sprite.h"

class TestScene :
    public UIElement
{
public:
    TestScene() : UIElement(0,0)
    {
        Sprite* sprite1 = new Sprite(250, 250, "square", RenderType::IMAGE, 500, 500, 0);
        sprite1->setRGBAModulation({ 255, 0, 0, 255 });
        addChild(sprite1);
        setTestHover(sprite1);

        Sprite* sprite1a = new Sprite(0, 0, "square", RenderType::IMAGE, 50, 50, 0);
        sprite1a->setRGBAModulation({ 0, 255, 0, 255 });
        sprite1->addChild(sprite1a);
        setTestHover(sprite1a);

        Sprite* sprite1b = new Sprite(100, 100, "square", RenderType::IMAGE, 50, 50, 0);
        sprite1b->setRGBAModulation({ 0, 0, 255, 255 });
        sprite1->addChild(sprite1b);
        setTestHover(sprite1b);

        Sprite* sprite2 = new Sprite(800, 250, "square", RenderType::IMAGE, 500, 500, 0);
        sprite2->setRGBAModulation({ 0, 255, 0, 255 });
        addChild(sprite2);
        setTestHover(sprite2);

        Sprite* sprite2a = new Sprite(0, 0, "square", RenderType::IMAGE, 400, 400, 0);
        sprite2a->setRGBAModulation({ 255, 255, 0, 255 });
        sprite2->addChild(sprite2a);
        setTestHover(sprite2a);

        Sprite* sprite2aa = new Sprite(0, 0, "square", RenderType::IMAGE, 300, 300, 0);
        sprite2aa->setRGBAModulation({ 0, 255, 255, 255 });
        sprite2a->addChild(sprite2aa);
        setTestHover(sprite2aa);
    }
    void setTestHover(Sprite* spr)
    {
        SDL_Color prevMod = spr->getRGBAModulation();
        spr->m_userEvents.onHoverEnter.addListener([spr](MouseEvent& mev)
            {
                spr->setRGBAModulation({ 255, 255, 255, 255 });
            });

        spr->m_userEvents.onHoverExit.addListener([spr, prevMod](MouseEvent& mev)
            {
                spr->setRGBAModulation(prevMod);
            });
    }
    void update(float dt) override
    {
        UIElement::update(dt);
    }
};

