#pragma once
#include "Item/Item.h"

class ItemGenerator :
    public Singleton<ItemGenerator>
{

public:
    void Initialize();



    Item* GetItem(const ITEMID& itemId);
  
};

#define ITEMGENERATOR (ItemGenerator::GetInstance())
