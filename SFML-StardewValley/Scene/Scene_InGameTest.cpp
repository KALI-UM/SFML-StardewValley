#include "pch.h"
#include "Scene_InGameTest.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileGrid.h"

#include "Player.h"
#include "Animator.h"
#include "PlayerStatusUi.h"
#include "Item/Hoe.h"


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
	m_TileView->SetTileViewIndex((int)TileLayer::Back, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)TileLayer::Object, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileObjectSystem = AddGameObject(m_UILayerIndex, new TileObjectSystem(m_TileModel));

	m_TileGrid = AddGameObject(2, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);
	m_TileGrid->SetIsVisible(false);

	m_Player = AddGameObject(3, new Player("Player"));
	m_PlayerStatusUi = AddGameObject(m_UILayerIndex, new PlayerStatusUi());
	m_TestItem = AddGameObject(3, new Hoe());

    return false;
}

void Scene_InGameTest::Enter()
{
	m_Player->GetHoe(m_TestItem);
	m_TestItem->GetPlayer(m_Player);
}

void Scene_InGameTest::Update(float dt)
{
	if (m_Player != nullptr)
	{
		GAME_MGR->SetViewCenter(0, m_Player->getPosition());
	}
	m_PlayerStatusUi->setPosition(sf::Vector2f(GAME_MGR->GetWindow()->getSize().x - 100, GAME_MGR->GetWindow()->getSize().y - 100));
}

void Scene_InGameTest::PostRender()
{
}
