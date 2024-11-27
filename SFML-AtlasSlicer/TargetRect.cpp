#include "pch.h"
#include "TargetRect.h"

bool TargetRect::Initialize()
{
	m_Rect = new DRectangle(sf::FloatRect(0, 0, 10, 10), ColorPalette::Magenta, 1);
	m_Rect->SetDebugDraw(false);
	SetDrawableObj(m_Rect);

	for (int j = 0; j < 100; j++)
		for (int i = 0; i < 100; i++)
		{
			m_SubRect[j][i] = new DRectangle(sf::FloatRect(0, 0, 0, 0), ColorPalette::DarkRed, 1);
			m_SubRect[j][i]->SetDebugDraw(false);
			SetDrawableObj(m_SubRect[j][i]);
		}

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
	m_Size = sf::FloatRect(rectTL, rectBR - rectTL);
	m_Rect->SetFloatRect(m_Size);

	SliceRect(subx, suby);
}

void TargetRect::SetRect(const sf::FloatRect& rect)
{
	m_Size = rect;
	m_Rect->SetFloatRect(m_Size);
	SliceRect(subx, suby);
}

void TargetRect::SliceRect(int x, int y)
{
	subx = x;
	suby = y;

	for (int j = 0; j < 100; j++)
		for (int i = 0; i < 100; i++)
		{
			m_SubRect[j][i]->SetFloatRect(sf::FloatRect(0, 0, 0, 0));
		}

	if (x == 1 && y == 1)return;
	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			m_SubRect[j][i]->SetFloatRect(sf::FloatRect(m_Size.getPosition().x + i * (m_Size.width /x),
				m_Size.getPosition().y + j * (m_Size.height / y), 
				m_Size.width / x, 
				m_Size.height / y));
		}
	}
}

const sf::FloatRect& TargetRect::GetSubRect(int x, int y)
{
	return m_SubRect[y][x]->GetGlobalBounds();
}


