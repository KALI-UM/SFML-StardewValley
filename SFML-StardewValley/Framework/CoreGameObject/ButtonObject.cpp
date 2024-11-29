#include "pch.h"
#include "ButtonObject.h"

ButtonObject::ButtonObject()
{
}

ButtonObject::~ButtonObject()
{
}

bool ButtonObject::Initialize()
{
	m_ButtonSprite = new DSprite("ui/button.png");
	m_ButtonSprite->SetOriginCenter();
	m_ButtonText = new DText("resource/CookieRun Black.ttf", "", 30);
	m_ButtonText->SetOutlineColor(sf::Color::Black);
	m_ButtonText->SetOutlineThickness(1);
	m_ButtonText->SetOriginCenter();
	SetDrawableObj(m_ButtonSprite);
	SetDrawableObj(m_ButtonText);

	return true;
}

void ButtonObject::Reset()
{
	m_ButtonSprite->SetColor(m_DefaultColor);
	m_ButtonArea = m_ButtonSprite->GetGlobalBounds();
	m_IsOverlaying = false;
	m_IsClicked = false;
}

void ButtonObject::Update(float dt)
{
	m_IsOverlaying = m_ButtonArea.contains({ (float)MOUSEPOS.x,(float)MOUSEPOS.y });
	m_IsClicked = m_IsOverlaying && INPUT_MGR->GetMouseDown(sf::Mouse::Left);

	if (m_IsOverlaying)
	{
		m_ButtonSprite->SetColor(m_OverlayColor);
		if (m_ButtonSprite->GetDebugDraw())
			m_ButtonSprite->GetDebugDraw()->setColor(ColorPalette::Red);
		if (m_WhenOverlay)
			m_WhenOverlay();
	}
	else
	{
		m_ButtonSprite->SetColor(m_DefaultColor);
		if (m_ButtonSprite->GetDebugDraw())
			m_ButtonSprite->GetDebugDraw()->setColor(ColorPalette::Blue);
	}

	if (m_IsClicked)
	{
		if (m_WhenClicked)
			m_WhenClicked();
	}
}

void ButtonObject::SetButtonPosition(const sf::Vector2f& pos)
{
	m_ButtonSprite->setPosition(pos);
	m_ButtonText->setPosition(pos);
	m_ButtonArea = m_ButtonSprite->GetGlobalBounds();
}

void ButtonObject::SetButtonText(const std::string& text)
{
	m_ButtonText->SetString(text);
	m_ButtonText->SetOriginCenter();
}

void ButtonObject::SetButtonColor(const sf::Color& on, const sf::Color& off)
{
	m_DefaultColor = off;
	m_OverlayColor = on;
}

bool ButtonObject::GetIsOveraying() const
{
	return m_IsOverlaying;
}

bool ButtonObject::GetIsClicked() const
{
	return m_IsClicked;
}

void ButtonObject::SetOverlayFunc(std::function<void()> func)
{
	m_WhenOverlay = func;
}

void ButtonObject::SetClickedFunc(std::function<void()> func)
{
	m_WhenClicked = func;
}
