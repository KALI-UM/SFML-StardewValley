#include "pch.h"
#include "ImguiWindowTab.h"
#include "D9SliceSprite.h"


bool ImguiWindowTab::Initialize()
{
	m_Bar = new D9SliceSprite();
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 1);

	SetDrawableObj(m_Bar);
	return false;
}

void ImguiWindowTab::Reset()
{
	m_Bar->SetTexture("ui/windowAtlas.png");
	m_Bar->SetCornerSize(32);
	m_Bar->SetSize({ 16 * 8 + 20, 16 * 20 });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));

	setPosition(16 * 16 + 40, 0);
	setScale(2, 2);
}

void ImguiWindowTab::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_UIViewIndex);
	m_HasFocus = false;
	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_HasFocus = true;
	}
	else
	{
		return;
	}
}
