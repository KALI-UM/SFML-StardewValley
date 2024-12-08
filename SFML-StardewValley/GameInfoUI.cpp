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
	return false;
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

	SetAspectRatioPosition(sf::Vector2f(1.0f, 0), sf::Vector2f(-10, 10));
	setScale({ 4, 4 });
}

void GameInfoUI::Update(float dt)
{
	m_ClockPointer->setLocalRotation(-180 + INGAMETIME->GetPassedDayTimeRatio() * 180);
}

void GameInfoUI::Release()
{
}
