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
	AnimationClips();
	SetDrawableObj(sprite);
	SetDrawableObj(effectSprite);
	sprite->SetDebugDraw(false);
	effectSprite->SetDebugDraw(false);

	return true;

}

void Hoe::ToolAction(Player* const player)
{
	//도구 애니메이션, 상호작용등을 구현해야.

	

	if (player->GetDirection() == Player::Direction::up &&player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->setScale({ 2.3f, 2.3f });
			effectSprite->setScale({ 2.3f,2.3f });
			sprite->SetIsVisible(true);
			effectSprite->SetIsVisible(true);
			sprite->setPosition({ player->getPosition().x, player->getPosition().y- 35.f });
			effectSprite->setPosition(player->getPosition().x + 1.f, player->getPosition().y - 42.f);
			animator.Play(&temp["Up"], true);
			effectAnimator.Play(&temp2["UpEffect"], true);
			sprite->SetPriorityType(DrawPriorityType::Custom, -1);
			effectSprite->SetPriorityType(DrawPriorityType::Custom, 0);
			
		}
	}
	if (player->GetDirection() == Player::Direction::down && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->setScale({2.3f, 2.3f});
			effectSprite->setScale({ 2.3f,2.3f });
			sprite->SetIsVisible(true);
			effectSprite->SetIsVisible(true);
			animator.Play(&temp["Down"], true);
			effectAnimator.Play(&temp2["DownEffect"], true);
			sprite->setPosition({ player->getPosition().x - 10.f, player->getPosition().y - 40.f });
			effectSprite->setPosition(player->getPosition().x + 12.f, player->getPosition().y- 7.f);
			effectSprite->SetPriorityType(DrawPriorityType::Custom, 3);
			sprite->SetPriorityType(DrawPriorityType::Custom, 2);
			
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
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({ "graphics/HoeDownEffect.png", {i * width, 0, width, height}});
		}
		temp2.insert({ "DownEffect", clip });
	} 
	{
		AnimationClip clip;
		clip.id = "UpEffect";
		clip.fps = 7;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 4; ++i)
		{
			clip.frames.push_back({ "graphics/HoeUpEffect.png", {i * 16, 0, 16, 20} });
		}
		temp2.insert({ "UpEffect", clip });
	}

	animator.SetTarget(sprite);
	effectAnimator.SetTarget(effectSprite);

	animator.AddEvent("Up", 1, [&]() {if (time == 0) { time = FRAMEWORK->GetTime(); } sprite->setPosition({ player->getPosition().x, player->getPosition().y + 10.f }); });
	animator.AddEvent("Down", 1, [&]() {if (time == 0) { time = FRAMEWORK->GetTime(); } sprite->setPosition({ player->getPosition().x, player->getPosition().y + 10.f }); });
	effectAnimator.AddEvent("DownEffect", 3, [&]() {effectSprite->SetIsVisible(false); });
	effectAnimator.AddEvent("UpEffect", 3, [&]() {effectSprite->SetIsVisible(false); });
}

void Hoe::Update(float dt)
{
	sf::Vector2f playerPos = player->getPosition();
	animator.Update(dt);
	effectAnimator.Update(dt);
	if (player->GetDirection() == Player::Direction::left && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			
			const TexRes& curr = TEXRESTABLE_MGR->GetTileTexRes(m_EquipTexIds[1]);
			
			sprite->SetTexture(curr.filepath, true);
			sprite->SetTextureRect(curr.texcoord);
			sprite->SetIsVisible(true);
			sprite->setPosition(playerPos);
			count += dt;
			//sprite->setRotation(-(std::numbers::pi * 10.f * count));
		}
	}
	if (player->GetDirection() == Player::Direction::right && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
			sprite->SetIsVisible(true);

		}
	}
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
