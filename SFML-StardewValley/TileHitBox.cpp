#include "pch.h"
#include "TileHitBox.h"

bool TileHitBox::Initialize()
{
    m_HitBoxRect = new DRectangle(m_HitBox, sf::Color::Cyan, 1.0f, sf::Color::Transparent);
    SetDrawableObj(m_HitBoxRect);
    return false;
}
