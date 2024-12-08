#include "pch.h"
#include "EventUI.h"
#include "D9SliceSprite.h"

EventUI::EventUI()
	:UIObject("EventUI")
{
}

EventUI::~EventUI()
{
}

bool EventUI::Initialize()
{
	m_Frame = new D9SliceSprite();
	SetDrawableObj(m_Frame);
	m_Frame->SetPriorityType(DrawPriorityType::Custom, 10);
	m_Frame->SetDebugDraw(false);

	m_Background = new DRectangle();
	SetDrawableObj(m_Background);
	m_Background->SetPriorityType(DrawPriorityType::Custom, 0);
	m_Background->SetDebugDraw(false);

	m_EventText = new DText();
	SetDrawableObj(m_EventText);
	m_EventText->SetPriorityType(DrawPriorityType::Custom, 3);
	m_EventText->SetDebugDraw(false);

	m_OK = new DText();
	SetDrawableObj(m_OK);
	m_OK->SetPriorityType(DrawPriorityType::Custom, 3);
	m_OK->SetDebugDraw(false);

	m_NO = new DText();
	SetDrawableObj(m_NO);
	m_NO->SetPriorityType(DrawPriorityType::Custom, 3);
	m_NO->SetDebugDraw(false);

	return true;
}

void EventUI::Reset()
{
	m_Frame->SetTexture("ui/Menu Elements.png");
	m_Frame->SetCornerSize(16);
	m_Frame->SetSize({ 16 * 10, 16 * 5 });
	m_Frame->SetAtlas(sf::FloatRect(0, 0, 16, 16), sf::FloatRect(32, 0, 16, 16), sf::FloatRect(48, 0, 16, 16),
		sf::FloatRect(0, 32, 16, 16), sf::FloatRect(0, 0, 0, 0), sf::FloatRect(48, 32, 16, 16),
		sf::FloatRect(0, 48, 16, 16), sf::FloatRect(32, 48, 16, 16), sf::FloatRect(48, 48, 16, 16));
	m_Frame->SetOrigin(OriginType::MC);


	m_Background->SetTexture("ui/Menu Elements.png");
	m_Background->SetTextureRect(sf::IntRect(16, 32, 16, 16));
	m_Background->SetSize({ 16 * 9, 16 * 4 });
	m_Background->SetOrigin(OriginType::MC);


	m_EventText->SetFont("fonts/DOSGothic.ttf");
	m_EventText->SetCharacterSize(25);
	m_EventText->setLocalPosition({ -16 * 4, -16 * 1.5f });
	m_EventText->setLocalScale({ 0.25f, 0.25f });
	m_EventText->SetColor(ColorPalette::Black);
	SetEventString(L"");

	m_OK->SetFont("fonts/DOSGothic.ttf");
	m_OK->SetCharacterSize(25);
	m_OK->setLocalPosition({ -16 * 4, 16 * 0 });
	m_OK->setLocalScale({ 0.25f, 0.25f });
	m_OK->SetColor(ColorPalette::DarkGray);
	m_OK->SetString(L"예");
	m_OK->SetOrigin(OriginType::TL);

	m_OKFunc = NULL;

	m_NO->SetFont("fonts/DOSGothic.ttf");
	m_NO->SetCharacterSize(25);
	m_NO->setLocalPosition({ -16 * 4, 16 * 1 });
	m_NO->setLocalScale({ 0.25f, 0.25f });
	m_NO->SetColor(ColorPalette::DarkGray);
	m_NO->SetString(L"아니요");
	m_NO->SetOrigin(OriginType::TL);

	m_NOFunc = NULL;

	setScale({ 4, 4 });
	SetAspectRatioPosition(sf::Vector2f(0.5f, 0.5f));

	SetIsActive(false);
	m_HasFocus = false;
}


void EventUI::Update(float dt)
{
	m_HasFocus = true;
	sf::Vector2f currmousepos = INPUT_MGR->GetMouseViewPos(m_UIViewIndex);
	if (m_Background->GetGlobalBounds().contains(currmousepos))
	{
		m_OK->SetColor(ColorPalette::DarkGray);
		m_NO->SetColor(ColorPalette::DarkGray);
		if (m_OK->GetGlobalBounds().contains(currmousepos))
		{
			m_OK->SetColor(ColorPalette::Black);
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left) && m_OKFunc)
			{
				m_OKFunc();
			}
		}
		if (m_NO->GetGlobalBounds().contains(currmousepos))
		{
			m_NO->SetColor(ColorPalette::Black);
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left) && m_NOFunc)
			{
				m_NOFunc();
			}
		}
	}
}


void EventUI::Release()
{
}

void EventUI::SetEventString(const std::wstring& str)
{
	m_EventText->SetString(str);
	m_EventText->SetOrigin(OriginType::TL);
}
