#include "pch.h"
#include "ItemGenerator.h"
#include "ItemDataTable.h"

#include "Item/Tool.h"
#include "Item/Crop.h"
#include "Item/Seed.h"


void ItemGenerator::Initialize()
{
}

Item* ItemGenerator::GetItem(const ITEMID& itemId)
{
    const ItemDataRes& itemdata = ITEMDATATABLE_MGR->GetItemDataRes(itemId);

	switch (itemdata.type)
	{	
	case ItemType::Tool:
		return new Tool(itemId);
	case ItemType::Foraging:
	case ItemType::Crop:
	case ItemType::Other:

		break;
	}
    return nullptr;
}
