#include "pch.h"
#include "Tool.h"

Tool::Tool(const ITEMID& itemId)
	:Item(itemId, ItemType::Tool)
{
}

void Tool::Use(Player* const player)
{
	ToolAction(player);
	
}


