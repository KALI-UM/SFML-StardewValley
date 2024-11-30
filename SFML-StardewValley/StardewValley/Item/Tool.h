#pragma once
#include "Item/Item.h"

class Player;
class Tool :
	public Item
{
protected:
	Tool(const ITEMID& itemId);
	
public:
	~Tool() {};

	void Use(Player* const player);

	virtual void ToolAction(Player* const player) = 0;								//자식에서 툴액션을 정의해줍셔
	int GetEnergyCost() const { return m_EnergyCost; }
protected:

	int m_EnergyCost = 2;
};

