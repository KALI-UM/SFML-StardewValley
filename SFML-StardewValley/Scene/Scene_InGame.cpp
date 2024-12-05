#include "pch.h"
#include "Scene_InGame.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"


Scene_InGame::Scene_InGame()
	:SceneBase("InGame", 5, 3)
{
}

Scene_InGame::~Scene_InGame()
{
}

bool Scene_InGame::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 1);
	SetLayerViewIndex(2, 1);
	SetLayerViewIndex(3, 1);
	SetLayerViewIndex(4, 2);
	SetViewNeedPriority(0, false);

	m_TileModel = AddGameObject(0, new TileModel((unsigned int)TileLayer::Max, { 80,64 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileViewIndex((int)TileLayer::Terrain, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)TileLayer::WaterEffect, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)TileLayer::Back, AddGameObject(1, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Object, AddGameObject(2, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Debug, AddGameObject(4, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)TileLayer::Light, AddGameObject(4, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileObjectSystem = AddGameObject(m_UILayerIndex, new TileObjectSystem(m_TileModel, m_TileView));

	m_TileGrid = AddGameObject(2, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);
	m_TileGrid->SetIsVisible(true);

	m_Back = AddGameObject(1, new TileObject("FarmHouseIndoorBack"));
	return false;
}

void Scene_InGame::Enter()
{
	m_TileObjectSystem->LoadTileLayerRawFile("datatables/TileObj/temp/FarmHouseIndoorTerraintex.csv");
	m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, m_Back);

	GAME_MGR->SetViewZoom(0, 0.5f);
	GAME_MGR->SetViewZoom(1, 0.5f);
	GAME_MGR->SetViewZoom(2, 0.5f);
	GAME_MGR->SetViewCenter(0, { 0, 0 });
	GAME_MGR->SetViewCenter(1, { 0, 0 });
	GAME_MGR->SetViewCenter(2, { 0, 0 });
}

void Scene_InGame::Update(float dt)
{
	if (INPUT_MGR->GetKey(sf::Keyboard::Num1))
	{
		CellIndex mousetile = m_TileObjectSystem->GetTileCoordinatedTileIndex(INPUT_MGR->GetMouseViewPos(0));
		std::cout << mousetile.x << "," << mousetile.y << std::endl;
		m_TileObjectSystem->ColorizePassableTile();
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::Num2))
	{
		m_TileObjectSystem->ColorizeInteractiveTile();
	}
}
