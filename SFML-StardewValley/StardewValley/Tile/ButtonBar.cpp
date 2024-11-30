#include "pch.h"
#include "Tile/ButtonBar.h"
#include "D9SliceSprite.h"
#include "DTile.h"

ButtonBar::ButtonBar(int viewIndex)
	:m_ViewIndex(viewIndex)
{
}

ButtonBar::~ButtonBar()
{
}

bool ButtonBar::Initialize()
{
	m_BarTexId = "ui/windowAtlas.png";
	m_Bar = new D9SliceSprite(m_BarTexId);
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 1);
	SetDrawableObj(m_Bar);

	m_Buttons.resize(m_ButtonCount);
	m_ButtonFuncs.resize(m_ButtonCount);
	for (int i = 0; i < m_ButtonCount; i++)
	{
		DTile* btt = new DTile();
		btt->setLocalPosition(sf::Vector2f(i % 8, i / 8) * 16.0f + sf::Vector2f(10, 16));
		m_Buttons[i] = btt;
		btt->SetPriorityType(DrawPriorityType::Custom, 2);
		SetDrawableObj(btt);
	}

	return true;
}

void ButtonBar::Reset()
{
	m_Bar->SetTexture(m_BarTexId);
	m_Bar->SetCornerSize(32);
	m_Bar->SetSize({ 16 * 8 + 20, 16 * 20 });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));
}

void ButtonBar::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);
	m_HasFocus = false;
	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_HasFocus = true;
	}
	else
	{
		return;
	}


	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		sf::Vector2f prevMousePos = GAME_MGR->GetScreenToViewPos(m_ViewIndex, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left));
		sf::FloatRect mouserect(currMousePos, currMousePos - prevMousePos);

		for (int i = 0; i < m_ButtonCount; i++)
		{
			if (m_Buttons[i]->GetGlobalBounds().intersects(mouserect))
			{
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::White);
				m_CurrButtonIndex = i;
				m_Buttons[m_CurrButtonIndex]->SetColor(ColorPalette::Gray);

				if (m_ButtonFuncs[m_CurrButtonIndex])
					m_ButtonFuncs[m_CurrButtonIndex]();
			}
		}
	}

	
}

void ButtonBar::Release()
{
}

void ButtonBar::SetButtonTex(const std::string& filepath, Action action, const sf::IntRect& rect)
{
	m_Buttons[(int)action]->SetTexture(filepath);
	m_Buttons[(int)action]->SetTexureRect(rect);
}

void ButtonBar::SetButtonFunc(Action action, std::function<void()> func)
{
	m_ButtonFuncs[(int)action] = func;
}
