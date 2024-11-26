#include "pch.h"
#include "TargetRect.h"

bool TargetRect::Initialize()
{
	m_Rect = new DRectangle(sf::FloatRect(0, 0, 10, 10), ColorPalette::Magenta, 2);
	m_Rect->SetDebugDraw(false);
	SetDrawableObj(m_Rect);
	return false;
}

void TargetRect::SetRect(const sf::Vector2f& start, const sf::Vector2f& end)
{
	sf::Vector2f rectTL;
	sf::Vector2f rectBR;
	rectTL.x = std::min(start.x, end.x);
	rectBR.x = std::max(start.x, end.x);
	rectTL.y = std::min(start.y, end.y);
	rectBR.y = std::max(start.y, end.y);

	m_Rect->SetFloatRect(sf::FloatRect(rectTL, rectBR - rectTL));
}

void TargetRect::SetRect(const sf::FloatRect& rect)
{
	m_Rect->SetFloatRect(rect);
}


