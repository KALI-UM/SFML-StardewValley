#pragma once
#include "GameObject.h"
class TargetRect :
    public GameObject
{
public:
    bool Initialize() override;
    
    void SetRect(const sf::Vector2f& start, const sf::Vector2f& end);
    void SetRect(const sf::FloatRect& rect);
    void SliceRect(int x, int y);
    const sf::FloatRect& GetSubRect(int x, int y);


    DRectangle* m_Rect;
    sf::FloatRect m_Size;
    DRectangle* m_SubRect[100][100];

    int subx = 1;
    int suby = 1;
};

