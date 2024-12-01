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
	effectSprite = new DSprite();
	effectSprite->SetIsVisible(false);
	effectSprite->setScale({ 1.3f,1.4f });
	AnimationClips();
	setScale({ 2.0f,2.3f });
	SetDrawableObj(sprite);
	SetDrawableObj(effectSprite);
	sprite->SetDebugDraw(false);
	effectSprite->SetDebugDraw(false);
	return true;
}

void Hoe::ToolAction(Player* const player)
{
	//���� �ִϸ��̼�, ��ȣ�ۿ���� �����ؾ�.

	

	if (player->GetDirection() == Player::Direction::up &&player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			animator.Play(&temp["Up"], true);
			sprite->SetPriorityType(DrawPriorityType::Custom, 0);
			
		}
	}
	if (player->GetDirection() == Player::Direction::left && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			count;
			setRotation(-(std::numbers::pi * 1.f * count));

		}
	}
	if (player->GetDirection() == Player::Direction::right && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);

		}
	}
	if (player->GetDirection() == Player::Direction::down && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);
			effectSprite->SetIsVisible(true);
			animator.Play(&temp["Down"], true);
			effectAnimator.Play(&temp2["DownEffect"], true);
			sprite->setPosition({ player->getPosition().x - 10.f, player->getPosition().y - 40.f });
			effectSprite->setPosition(player->getPosition().x + 10.f, player->getPosition().y +10.f);
			effectSprite->SetPriorityType(DrawPriorityType::Custom, 3);
			sprite->SetPriorityType(DrawPriorityType::Custom, 2);
			std::cout << value << std::endl;
		}
	}
}

void Hoe::AnimationClips()
{

	int width = 30;
	int height = 30;
	{
		AnimationClip clip;
		clip.loadFromTexId("Up", AnimationLoopTypes::Single, 4, m_EquipTexIds[2]);
		temp.insert({ "Up", clip });
	}
	{
		AnimationClip clip;
		clip.loadFromTexId("Down", AnimationLoopTypes::Single, 6, m_EquipTexIds[0]);
		temp.insert({ "Down", clip });
	}
	{
		AnimationClip clip;
		clip.id = "DownEffect";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({ "graphics/HoeDownEffect.png", {i * width, 0, width, height}});
		}
		temp2.insert({ "DownEffect", clip });
	}

	animator.SetTarget(sprite);
	effectAnimator.SetTarget(effectSprite);

	animator.AddEvent("Up", 1, [&]() {if (time == 0) { time = FRAMEWORK->GetTime(); } });
	animator.AddEvent("Down", 1, [&]() {if (time == 0) { time = FRAMEWORK->GetTime(); } sprite->setPosition({ player->getPosition().x, player->getPosition().y + 10.f }); });
	effectAnimator.AddEvent("DownEffect", 3, [&]() {effectSprite->SetIsVisible(false); });
}

void Hoe::Update(float dt)
{
	sf::Vector2f playerPos = player->getPosition();
	animator.Update(dt);
	effectAnimator.Update(dt);

	if (time == 0) {
		return;
	}
	if (delay + time < FRAMEWORK->GetTime()) {
		sprite->SetIsVisible(false);
		time = 0.f;
	}
	
	
}

void Hoe::GetPlayer(Player* player) {
	this->player = player;
}
