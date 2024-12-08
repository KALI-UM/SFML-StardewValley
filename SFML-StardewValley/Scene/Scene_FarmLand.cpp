#include "pch.h"
#include "Scene_FarmLand.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"

Scene_FarmLand::Scene_FarmLand()
	:Scene_InGame("FarmLand")
{
}

Scene_FarmLand::~Scene_FarmLand()
{
}

bool Scene_FarmLand::Initialize()
{
	bool result = Scene_InGame::Initialize();

	m_Back = AddGameObject(1, new TileObject("Back"));
	m_FarmHouse = AddGameObject(2, new TileObject("FarmHouse"));


	m_TerrainFilepath = "datatables/TileObj/temp/Terraintex.csv";
	return result;
}

void Scene_FarmLand::Enter()
{
	Scene_InGame::Enter();
	m_TileObjectSystem->SetTileObject(TileObjLayer::Buildings, {59,10 }, m_FarmHouse);
}

void Scene_FarmLand::Update(float dt)
{
	Scene_InGame::Update(dt);
}
