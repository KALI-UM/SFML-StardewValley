#pragma once
#include "GameObject.h"
class TargetRect :
    public GameObject
{
public:
    bool Initialize() override;
    
    void SetRect(const sf::Vector2f& start, const sf::Vector2f& end);
    void SetRect(const sf::FloatRect& rect);

    DRectangle* m_Rect;
};

