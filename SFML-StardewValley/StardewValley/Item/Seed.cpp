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
	//���� �ɴ� �� ��ȣ�ۿ� ���� �����ؾ�.
}
