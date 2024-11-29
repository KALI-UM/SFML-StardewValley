#pragma once
#include "Item/Item.h"

class Player;
class Crop :
    public Item
{
protected:
	Crop(const ITEMID& itemId);
public:
	~Crop() {};

	void Use(Player* const player);
	virtual void CropAction(Player* const player) = 0;
};

