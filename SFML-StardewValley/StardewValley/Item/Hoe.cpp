#include "pch.h"
#include "Hoe.h"
#include "Player.h"
#include "TexCoordTable.h"
#include <numbers>

Hoe::Hoe()
	:Tool("Hoe")
{

}

Hoe::~Hoe()
{

}

bool Hoe::Initialize()
{
	Item::Initialize();
	sprite = new DSprite();
	sprite->SetIsVisible(false);
	AnimationClips();
	setScale({ 2.0f,2.3f });
	SetDrawableObj(sprite);
	
	return true;
}

void Hoe::ToolAction(Player* const player)
{
	//도구 애니메이션, 상호작용등을 구현해야.

	sf::Vector2f playerPos = player->getPosition();
	setPosition(Utils::Lerp({ playerPos.x-20.f, playerPos.y-80.f}, {playerPos.x, playerPos.y+10.f},3.f,true));

	if (player->GetDirection() == Player::Direction::up) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			animator.Play(&temp["Up"], true);
		}
	}
	if (player->GetDirection() == Player::Direction::left) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			setScale({ -2.0f, 2.3f });
			count;
			setRotation(-(std::numbers::pi * 1.f * count));

		}
	}
	if (player->GetDirection() == Player::Direction::right) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
		
		}
	}
	if (player->GetDirection() == Player::Direction::down) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			animator.Play(&temp["Down"], true);
			sprite->SetPriorityType(DrawPriorityType::Custom, 1);
		}
	}

}

void Hoe::AnimationClips()
{

	{
		AnimationClip clip;
		clip.loadFromTexId("Up", AnimationLoopTypes::Single, 4, m_EquipTexIds[2]);
		temp.insert({ "Up", clip });
	}
	{
		AnimationClip clip;
		clip.loadFromTexId("Down", AnimationLoopTypes::Single, 4, m_EquipTexIds[0]);
		temp.insert({ "Down", clip });
	}


	animator.SetTarget(sprite);

	animator.AddEvent("Up", 1, [&]() {time = FRAMEWORK->GetTime(); });
	animator.AddEvent("Down", 1, [&]() {time = FRAMEWORK->GetTime();});

}

void Hoe::Update(float dt)
{
	animator.Update(dt);
	
	if (time == 0) {
		return;
	}
	/*if (delay < time) {
		sprite->SetIsVisible(false);
		time = 0.f;
	}*/
}
