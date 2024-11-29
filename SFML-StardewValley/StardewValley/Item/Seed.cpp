#include "pch.h"
#include "Seed.h"

const std::string Seed::m_SeedId = "Seed";

Seed::Seed(const std::string& harvestName)
	:Crop(harvestName+ m_SeedId)
{
}

Seed::~Seed()
{
}

void Seed::CropAction(Player* const player)
{
	//땅에 심는 등 상호작용 등을 구현해야.
}
