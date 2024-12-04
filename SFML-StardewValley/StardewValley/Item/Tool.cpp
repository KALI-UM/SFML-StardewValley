#include "pch.h"
#include "Tool.h"
#include "Player.h"

Tool::Tool(const ITEMID& itemId)
	:Item(itemId, ItemType::Tool)
{
}

bool Tool::Initialize()
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

void Tool::Use(Player* const player)
{
	
		if (player->GetDirection() == Player::Direction::up && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
				sprite->setScale({ 2.3f, 2.3f });
				effectSprite->setScale({ 2.3f,2.3f });
				sprite->SetIsVisible(true);
				effectSprite->SetIsVisible(true);
				sprite->setPosition(player->getPosition().x, player->getPosition().y - 45.f);
				effectSprite->setPosition(player->getPosition().x, player->getPosition().y - 40.f);
				animator.Play(&temp[m_ItemId+"Up"], true);
				effectAnimator.Play(&temp["ToolsUpEffect"], true);
				sprite->SetPriorityType(DrawPriorityType::Custom, -1);
				effectSprite->SetPriorityType(DrawPriorityType::Custom, 0);
				if (m_ItemId == "Scythe") {
					sprite->setPosition(player->getPosition().x +18.f, player->getPosition().y - 12.f);
					effectSprite->SetIsVisible(false);
				}
			}
		}
		if (player->GetDirection() == Player::Direction::left && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
				sprite->SetIsVisible(true);
				sprite->setScale({ -2.5f, 2.5f });
				effectSprite->setScale({ -2.8f,2.8f });
				sprite->SetIsVisible(true);
				effectSprite->SetIsVisible(true);
				sprite->setPosition(player->getPosition().x - 4.f, player->getPosition().y - 6.f);
				effectSprite->setPosition(player->getPosition().x - 8.f, player->getPosition().y - 6.f);
				animator.Play(&temp[m_ItemId+"Side"], true);
				effectAnimator.Play(&temp["ToolsSideEffect"], true);
				sprite->SetPriorityType(DrawPriorityType::Custom, 2);
				effectSprite->SetPriorityType(DrawPriorityType::Custom, 3);
				if (m_ItemId == "Scythe") {
					sprite->setPosition(player->getPosition().x + 10.f, player->getPosition().y + 25.f);
					effectSprite->SetIsVisible(false);
				}
			}
		}
		if (player->GetDirection() == Player::Direction::right && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
				sprite->SetIsVisible(true);
				sprite->setScale({ 2.5f, 2.5f });
				effectSprite->setScale({ 2.8f,2.8f });
				sprite->SetIsVisible(true);
				effectSprite->SetIsVisible(true);
				sprite->setPosition(player->getPosition().x + 4.f, player->getPosition().y - 6.f);
				effectSprite->setPosition(player->getPosition().x + 8.f, player->getPosition().y - 6.f);
				animator.Play(&temp[m_ItemId+"Side"], true);
				effectAnimator.Play(&temp["ToolsSideEffect"], true);
				sprite->SetPriorityType(DrawPriorityType::Custom, 2);
				effectSprite->SetPriorityType(DrawPriorityType::Custom, 3);
				if (m_ItemId == "Scythe") {
					sprite->setPosition(player->getPosition().x + 10.f, player->getPosition().y + 25.f);
					effectSprite->SetIsVisible(false);
				}
			}
		}
		if (player->GetDirection() == Player::Direction::down && player->GetIsVisibleItem() == Player::IsVisibleItem::visibleItem) {
			if (INPUT_MGR->GetMouseDown(sf::Mouse::Left)) {
				sprite->setScale({ 2.5f, 2.5f });
				effectSprite->setScale({ 2.f,2.5f });
				sprite->SetIsVisible(true);
				effectSprite->SetIsVisible(true);
				animator.Play(&temp[m_ItemId+"Down"], true);
				effectAnimator.Play(&temp["ToolsDownEffect"], true);
				sprite->setPosition(player->getPosition().x - 8.f, player->getPosition().y);
				effectSprite->setPosition(player->getPosition().x + 10.f, player->getPosition().y - 7.f);
				effectSprite->SetPriorityType(DrawPriorityType::Custom, 3);
				sprite->SetPriorityType(DrawPriorityType::Custom, 2);
				if (m_ItemId == "Scythe") {
					sprite->setPosition(player->getPosition().x +10.f, player->getPosition().y+ 25.f);
					effectSprite->SetIsVisible(false);
				}
			}
		}
	

}

void Tool::AnimationClips()
{
	rapidcsv::Document doc("datatables/AnimationDataTable/Item-ToolsAnimationFileList.csv", rapidcsv::LabelParams(-1, -1));
	std::vector<std::string> animationfilepath = doc.GetColumn<std::string>(0);
	AnimationClip clip;
	for (auto& curr : animationfilepath)
	{
		ANI_CLIP_MGR->LoadByFilepath(curr, clip.GetIdFromFilepath(curr));
		clip.loadFromFile(curr);
		temp.insert({ clip.GetIdFromFilepath(curr),  clip });

		animator.AddEvent(clip.GetIdFromFilepath(curr), 6, [&]() {sprite->SetIsVisible(false); });
		effectAnimator.AddEvent(clip.GetIdFromFilepath(curr), 6, [&]() {effectSprite->SetIsVisible(false); });
	}
	animator.SetTarget(sprite);
	effectAnimator.SetTarget(effectSprite);

	animator.AddEvent("ScytheUp", 5, [&]() {sprite->SetIsVisible(false); });
	animator.AddEvent("ScytheSide", 5, [&]() {sprite->SetIsVisible(false); });
	animator.AddEvent("ScytheDown", 5, [&]() {sprite->SetIsVisible(false); });
	effectAnimator.AddEvent("ToolsDownEffect", 3, [&]() {effectSprite->SetIsVisible(false); });
}

void Tool::Update(float dt)
{
	animator.Update(dt);
	effectAnimator.Update(dt);
}

void Tool::GetPlayer(Player* player) {
	this->player = player;
}


