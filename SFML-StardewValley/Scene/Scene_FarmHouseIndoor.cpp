#include "pch.h"
#include "Scene_FarmHouseIndoor.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"

Scene_FarmHouseIndoor::Scene_FarmHouseIndoor()
	:Scene_InGame("FarmHouseIndoor")
{
}

Scene_FarmHouseIndoor::~Scene_FarmHouseIndoor()
{
}

bool Scene_FarmHouseIndoor::Initialize()
{
	bool result = Scene_InGame::Initialize();
	m_MapSize = { 16 * 12, 16 * 12 };

	//m_TerrainFilepath = "datatables/TileObj/temp/FarmHouseIndoorTerraintex.csv";

	m_SunsetLightColor = ColorPalette::Transparent;
	return result;
}

void Scene_FarmHouseIndoor::Enter()
{
	Scene_InGame::Enter();
	//m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, "FarmHouseIndoorBack");
	//m_TileObjectSystem->SetTileObject(TileObjLayer::AlwaysFront, { 0,0 }, "FarmHouseIndoorFront");

	m_IsPlayerInHouse = true;
}

void Scene_FarmHouseIndoor::Update(float dt)
{
	Scene_InGame::Update(dt);
}

void Scene_FarmHouseIndoor::Exit()
{
	Scene_InGame::Exit();
}
