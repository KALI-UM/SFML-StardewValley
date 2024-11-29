#pragma once
#include "Item/Tool.h"

class Hoe :
    public Tool
{
public:
    Hoe();
    ~Hoe();

    void ToolAction(Player* const player);
};

