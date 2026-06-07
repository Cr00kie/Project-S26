#pragma once
#include "UIElement.h"
#include "../Tools/ReactiveProperty.h"
#include "Sprite.h"

class Slider :
    public UIElement
{
private:
    ReactiveProperty<float> m_value;
    Sprite* m_handle;
    Sprite* m_bar;
    Subscription m_holdSubscription;

    void onMouseHeld(MouseEvent& mev);

public:
    Slider(float x, float y, float iniValue = 0, float width = 0, float height = 0, float rotation = 0);
    ~Slider();

    inline void setValue(float value);
    inline float getValue() const { return m_value; }
    // Subscribe to changes in the reactive property by providing a callback function
    Subscription addValueChangedListener(std::function<void(const float&)> callback)
    {
        return m_value.subscribe(callback);
    }
    template<typename T, typename Method>
    Subscription addValueChangedListener(T* obj, Method method)
    {
        return addValueChangedListener([obj, method](const float& ev) { (obj->*method)(ev); });
    }

    Sprite* getHandle() const { return m_handle; }
    Sprite* getBar() const { return m_bar; }
};

