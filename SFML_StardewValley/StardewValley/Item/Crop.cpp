#include "pch.h"
#include "Crop.h"

Crop::Crop(const ITEMID& itemId)
	:Item(itemId, ItemType::Crop)
{
}

void Crop::Use(Player* const player)
{
	CropAction(player);
}
