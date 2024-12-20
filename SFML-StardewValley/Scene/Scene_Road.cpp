#include "pch.h"
#include "Scene_Road.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"

Scene_Road::Scene_Road()
    :Scene_InGame("Road")
{
}

Scene_Road::~Scene_Road()
{
}

bool Scene_Road::Initialize()
{
	bool result = Scene_InGame::Initialize();
	m_MapSize = { 16 * 32, 16 * 32 };
	//m_TerrainFilepath = "datatables/TileObj/temp/RoadTerraintex.csv";
	return result;
}

void Scene_Road::Enter()
{
	Scene_InGame::Enter();
	//m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, "RoadBack");
}

void Scene_Road::Update(float dt)
{
	Scene_InGame::Update(dt);
}

void Scene_Road::Exit()
{
	Scene_InGame::Exit();
}
