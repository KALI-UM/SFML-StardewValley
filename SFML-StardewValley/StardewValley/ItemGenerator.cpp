#include "pch.h"
#include "ItemGenerator.h"
#include "Scene_InGame.h"
#include "ItemDataTable.h"
#include "Item/DropItem.h"

#include "Item/Tool.h"
#include "Item/Crop.h"
#include "Item/Seed.h"
#include "Item/Foraging.h"



void ItemGenerator::Enter(Scene_InGame* scene)
{
	m_CurrInGameScene = scene;
	m_Player = scene->GetPlayer();
}

Item* ItemGenerator::GetItem(const ITEMID& itemId)
{
    const ItemDataRes& itemdata = ITEMDATATABLE_MGR->GetItemDataRes(itemId);

	switch (itemdata.type)
	{	
	case ItemType::Tool:
		return new Tool(itemId);
	case ItemType::Foraging:
		return new Foraging(itemId);
	case ItemType::Crop:
	case ItemType::Other:

		break;
	}
    return nullptr;
}

void ItemGenerator::DropItemToMap(const ITEMID& id, const sf::Vector2f& pos)
{
	DropItem* item = m_CurrInGameScene->m_DropItems.Take();
	item->Load(id);
	item->FloatingEffect(pos);
}
