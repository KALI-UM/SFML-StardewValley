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
	:SceneBase("InGameTest", 4, 2)
{
}

Scene_InGameTest::~Scene_InGameTest()
{
}

bool Scene_InGameTest::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 0);
	SetLayerViewIndex(2, 1);
	SetLayerViewIndex(3, 1);
	SetViewNeedPriority(0, false);

	m_TileModel = AddGameObject(0, new TileModel((unsigned int)TileLayer::Max, {80,64}, {16,16}));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileViewIndex((int)TileLayer::Terrain, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)TileLayer::WaterEffect, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)TileLayer::Back, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Object, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileObjectSystem = AddGameObject(m_UILayerIndex, new TileObjectSystem(m_TileModel, m_TileView));

	m_TileGrid = AddGameObject(2, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);
	m_TileGrid->SetIsVisible(true);

	m_Player = AddGameObject(3, new Player("Player"));
	m_PlayerStatusUi = AddGameObject(m_UILayerIndex, new PlayerStatusUi());
	m_TestItem = AddGameObject(3, new Tool("Scythe"));

	m_TestTObj = AddGameObject(0, new TileObject("Back"));

    return false;
}

void Scene_InGameTest::Enter()
{
	m_Player->GetTool(dynamic_cast<Tool*>(m_TestItem));
	m_Player->GetItemType(dynamic_cast<Tool*>(m_TestItem)->m_ItemType);
	dynamic_cast<Tool*>(m_TestItem)->GetPlayer(m_Player);


	m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, m_TestTObj);
	m_Player->SetTileSystem(m_TileObjectSystem);

	GAME_MGR->SetViewZoom(0, 0.5f);
	GAME_MGR->SetViewZoom(1, 0.5f);
}

void Scene_InGameTest::Update(float dt)
{
	if (m_Player != nullptr)
	{
		GAME_MGR->SetViewCenter(0, m_Player->getPosition());
		GAME_MGR->SetViewCenter(1, m_Player->getPosition());
	}
	m_PlayerStatusUi->setPosition(sf::Vector2f(GAME_MGR->GetWindow()->getSize().x - 100, GAME_MGR->GetWindow()->getSize().y - 100));
}


