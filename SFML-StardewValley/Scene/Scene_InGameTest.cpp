#include "pch.h"
#include "Scene_InGameTest.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileGrid.h"

#include "Player.h"
#include "PlayerStatusUi.h"
#include "Item/Tool.h"

#include "Tile/TileObject.h"


Scene_InGameTest::Scene_InGameTest()
	:SceneBase("InGameTest", 5, 3)
{
}

Scene_InGameTest::~Scene_InGameTest()
{
}

bool Scene_InGameTest::Initialize()
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

	m_Player = AddGameObject(3, new Player("Player"));
	m_PlayerStatusUi = AddGameObject(m_UILayerIndex, new PlayerStatusUi());
	m_TestItem = AddGameObject(3, new Tool("Scythe"));

	m_TestTObj = AddGameObject(0, new TileObject("Back"));
	m_TestTObj2 = AddGameObject(2, new TileObject("TreeStump01"));
	m_TestTObj3 = AddGameObject(2, new TileObject("FarmHouse"));
	m_TestTObj4 = AddGameObject(2, new TileObject("GreenHouse"));
	m_TestTObj5 = AddGameObject(2, new TileObject("Rock"));

	return false;
}

void Scene_InGameTest::Enter()
{
	m_Player->GetTool(dynamic_cast<Tool*>(m_TestItem));
	m_Player->GetItemType(dynamic_cast<Tool*>(m_TestItem)->m_ItemType);
	dynamic_cast<Tool*>(m_TestItem)->GetPlayer(m_Player);


	m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, m_TestTObj);
	m_TileObjectSystem->SetTileObject(TileObjLayer::Paths, { 10,10 }, m_TestTObj2);
	m_TileObjectSystem->SetTileObject(TileObjLayer::Buildings, { 20,20 }, m_TestTObj3);
	m_TileObjectSystem->SetTileObject(TileObjLayer::Buildings, { 40,20 }, m_TestTObj4);
	m_TileObjectSystem->SetTileObject(TileObjLayer::Paths, { 30,30 }, m_TestTObj5);
	m_Player->SetTileSystem(m_TileObjectSystem);

	GAME_MGR->SetViewZoom(0, 0.5f);
	GAME_MGR->SetViewZoom(1, 0.5f);
	GAME_MGR->SetViewZoom(2, 0.5f);
}

void Scene_InGameTest::Update(float dt)
{
	if (m_Player != nullptr)
	{
		GAME_MGR->SetViewCenter(0, m_Player->getPosition());
		GAME_MGR->SetViewCenter(1, m_Player->getPosition());
		GAME_MGR->SetViewCenter(2, m_Player->getPosition());
	}
	m_PlayerStatusUi->setPosition(sf::Vector2f(GAME_MGR->GetWindow()->getSize().x - 100, GAME_MGR->GetWindow()->getSize().y - 100));

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


