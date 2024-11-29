#include "pch.h"
#include "Tool.h"

Tool::Tool(const ITEMID& itemId)
	:Item(itemId, ItemType::Tool)
{
}

void Tool::Use(Player* const player)
{
	UseEnergy(player);
	ToolAction(player);
}

void Tool::UseEnergy(Player* const player)
{
	//에너지 소모하는 함수 주세요
}
