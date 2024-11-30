#include "pch.h"
#include "PlayerStatusUi.h"
#include "Player.h"
#include <numbers>

PlayerStatusUi::PlayerStatusUi()
{
}

PlayerStatusUi::~PlayerStatusUi()
{
}

bool PlayerStatusUi::Initialize()
{
	
	StaminaUi = new DSprite("graphics/Stamina.png");
	Staminabar = new DRectangle(sf::FloatRect(0, 0, 12, 85), ColorPalette::Transparent, 1, ColorPalette::Green);
	dashboardInfo = new DSprite("graphics/Stardew Valley - Clock.png");
	ClockHands = new DSprite("graphics/Stardew Valley_Clock_hands.png");

	SetDrawableObj(StaminaUi);
	StaminaUi->SetPriorityType(DrawPriorityType::Custom, 0);
	SetDrawableObj(Staminabar);
	Staminabar->SetPriorityType(DrawPriorityType::Custom, 1);
	SetDrawableObj(dashboardInfo);
	dashboardInfo->SetPriorityType(DrawPriorityType::Custom, 2);
	SetDrawableObj(ClockHands);
	ClockHands->SetPriorityType(DrawPriorityType::Custom, 3);

	StaminaUi->SetDebugDraw(false);
	Staminabar->SetDebugDraw(false);
	dashboardInfo->SetDebugDraw(false);
	ClockHands->SetDebugDraw(false);
	return true;
}

void PlayerStatusUi::Reset()
{	
	StaminaUi->SetOrigin(OriginType::BC);
	StaminaUi->setScale({ 2.f,2.f });
	Staminabar->setPosition(StaminaUi->getPosition());
	Staminabar->SetOrigin(OriginType::TC);
	Staminabar->setScale({ 1,-1 });
	player = SCENE_MGR->GetCurrentScene()->FindGameObject<Player>(0, "Player");
	dashboardInfo->setScale({ 2.f, 2.f });
	dashboardInfo->setPosition(StaminaUi->getPosition().x- 100, StaminaUi->getPosition().y - 700.f);
	ClockHands->setPosition(dashboardInfo->getPosition().x + 41, dashboardInfo->getPosition().y+40);
	ClockHands->SetOrigin(OriginType::BC);
	ClockHands->setScale({ 2.f, -2.f });
}

void PlayerStatusUi::Update(float dt)
{
	Staminabar->SetSize({ 12.f, 85 * player->Staminagauge() });

	if (Staminabar->GetSize().y < 42.5f) {
		Staminabar->SetFillColor(ColorPalette::Yellow);
	}
	if (Staminabar->GetSize().y < 17.f) {
		Staminabar->SetFillColor(ColorPalette::Red);
	}
	if (Staminabar->GetSize().y == 85.f) {
		Staminabar->SetFillColor(ColorPalette::Green);
	}
    count += dt;
	ClockHands->setRotation(std::numbers::pi * 1.f * count);
	std::cout << "(" << ClockHands->getRotation() << ")" << std::endl;
}

void PlayerStatusUi::UpdateIdle(float dt)
{
}

void PlayerStatusUi::UpdateMove(float dt)
{
}

void PlayerStatusUi::UpdateInter(float dt)
{
}

void PlayerStatusUi::LateUpdate(float dt)
{
}

void PlayerStatusUi::FixedUpdate(float dt)
{
}

void PlayerStatusUi::PreRender()
{
}

void PlayerStatusUi::PostRender()
{
}

void PlayerStatusUi::ImGuiUpdate()
{
}

void PlayerStatusUi::Release()
{
}
