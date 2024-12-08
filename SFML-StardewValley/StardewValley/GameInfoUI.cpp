#include "pch.h"
#include "GameInfoUI.h"
#include "TexCoordTable.h"
#include "InGameTime.h"

GameInfoUI::GameInfoUI()
{
}

GameInfoUI::~GameInfoUI()
{
}

bool GameInfoUI::Initialize()
{
	m_ClockBody = new DSprite();
	SetDrawableObj(m_ClockBody);
	m_ClockBody->SetPriorityType(DrawPriorityType::Custom, 0);
	m_ClockBody->SetDebugDraw(false);

	m_ClockPointer = new DSprite();
	SetDrawableObj(m_ClockPointer);
	m_ClockPointer->SetPriorityType(DrawPriorityType::Custom, 1);
	m_ClockPointer->SetDebugDraw(false);

	m_Day = new DText();
	SetDrawableObj(m_Day);
	m_Day->SetPriorityType(DrawPriorityType::Custom, 1);
	m_Day->SetDebugDraw(false);

	m_Time = new DText();
	SetDrawableObj(m_Time);
	m_Time->SetPriorityType(DrawPriorityType::Custom, 1);
	m_Time->SetDebugDraw(false);

	m_Gold = new DText();
	SetDrawableObj(m_Gold);
	m_Gold->SetPriorityType(DrawPriorityType::Custom, 1);
	m_Gold->SetDebugDraw(false);

	return true;
}

void GameInfoUI::Reset()
{
	auto& texresbody = TEXRESTABLE_MGR->GetTileTexRes("Clock.png#ClockUIBody");
	m_ClockBody->SetTexture(texresbody.filepath);
	m_ClockBody->SetTextureRect(texresbody.texcoord);
	m_ClockBody->SetOrigin(OriginType::TR);

	auto& texrespointer = TEXRESTABLE_MGR->GetTileTexRes("Clock.png#ClockUIPointer");
	m_ClockPointer->SetTexture(texrespointer.filepath);
	m_ClockPointer->SetTextureRect(texrespointer.texcoord);
	m_ClockPointer->SetOrigin(OriginType::BC);
	m_ClockPointer->setLocalPosition({ -16 * 3 - 2, 20 });

	m_Day->SetFont("fonts/DOSGothic.ttf");
	m_Day->SetCharacterSize(30);
	m_Day->setLocalPosition({ -16 * 2.5f, 5});
	m_Day->setLocalScale({ 0.25f, 0.25f });
	m_Day->SetColor(ColorPalette::Black);

	m_Time->SetFont("fonts/DOSGothic.ttf");
	m_Time->SetCharacterSize(30);
	m_Time->setLocalPosition({ -16 * 2.5f, 27 });
	m_Time->setLocalScale({ 0.25f, 0.25f });
	m_Time->SetColor(ColorPalette::Black);

	SetAspectRatioPosition(sf::Vector2f(1.0f, 0), sf::Vector2f(-10, 10));
	setScale({ 4, 4 });
}

void GameInfoUI::Update(float dt)
{
	m_ClockPointer->setLocalRotation(-180 + INGAMETIME->GetPassedDayTimeRatio() * 180);
	const auto& time = INGAMETIME->GetInGameTime();
	m_Day->SetString(std::to_string(time.month) + "." + std::to_string(time.day));
	m_Day->SetOrigin(OriginType::TL);
	m_Time->SetString(std::to_string(time.hour) + ":" + std::to_string(time.min));
	m_Time->SetOrigin(OriginType::TL);
}

void GameInfoUI::Release()
{
}
