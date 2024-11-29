#pragma once
#include "Item/Crop.h"

class Seed :
    public Crop
{
public:
    Seed(const std::string& harvestName);
    ~Seed();

    void CropAction(Player* const player);
protected:


   static const std::string m_SeedId;
};

