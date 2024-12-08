#pragma once
#include "Item/Item.h"

class Player;
class Foraging :
    public Item
{
public:
	Foraging(const ITEMID& itemId);
	~Foraging() {};

	void Use(Player* const player);

};

