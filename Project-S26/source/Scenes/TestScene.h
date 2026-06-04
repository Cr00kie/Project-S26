#pragma once
#include "../UI/UIElement.h"
#include "../UI/BasicUI/Sprite.h"

class TestScene :
    public UIElement
{
public:
    TestScene() : UIElement(0,0)
    {
        Sprite* sprite = new Sprite(250, 250, "test", RenderType::IMAGE, 500, 500, 45);
        sprite->setRGBAModulation({ 255, 0, 0, 255 });
        sprite->m_userEvents.onHoverEnter.addListener([sprite](MouseEvent& mev)
            {
                sprite->setTexture("square");
            });
        sprite->m_userEvents.onHoverExit.addListener([sprite](MouseEvent& mev)
            {
                sprite->setTexture("test");
            });
        addChild(sprite);
    }
    void update(float dt) override
    {
        UIElement::update(dt);
    }
};

