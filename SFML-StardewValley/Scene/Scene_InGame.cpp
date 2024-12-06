#include "pch.h"
#include "Scene_InGame.h"
#include "InGameTime.h"
#include "Variables.h"
#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"

#include "Player.h"

Scene_InGame::Scene_InGame(const std::string& name)
	:SceneBase("InGame::" + name, 6, 4)
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
	SetLayerViewIndex(5, 3);

	SetViewNeedPriority(0, false);
	SetViewNeedPriority(2, false);
	//Terrain, WaterEffect, Back == 0 (SelfPriority)
	//Object, Player == 1
	//Front, Debug == 2 (SelfPriority)
	//Light == 3

	m_TileModel = AddGameObject(0, new TileModel((unsigned int)TileLayer::Max, { 80,64 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileViewIndex((int)TileLayer::Terrain, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)TileLayer::WaterEffect, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)TileLayer::Back, AddGameObject(1, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Object, AddGameObject(2, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Front, AddGameObject(4, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)TileLayer::Debug, AddGameObject(4, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)TileLayer::Light, AddGameObject(5, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileObjectSystem = AddGameObject(m_UILayerIndex, new TileObjectSystem(m_TileModel, m_TileView));

	m_TileGrid = AddGameObject(2, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);

	m_Player = AddGameObject(3, new Player("Player"));

	return true;
}

void Scene_InGame::Enter()
{
	SetPlayMode(InGamePlayMode::Play);

	m_TileObjectSystem->LoadTileLayerRawFile(m_TerrainFilepath);
	m_TileObjectSystem->SetTileObject(TileObjLayer::Back, { 0,0 }, m_Back);

	m_Player->SetTileSystem(m_TileObjectSystem);


	m_Player->setPosition(Variables::m_EnterPoint.To<float>()*16.0f);

	GAME_MGR->SetViewZoom(0, 0.5f);
	GAME_MGR->SetViewZoom(1, 0.5f);
	GAME_MGR->SetViewZoom(2, 0.5f);

}

void Scene_InGame::Update(float dt)
{
	UpdatePlayMode(dt);
	INGAMETIME->Update(dt);
}

void Scene_InGame::Exit()
{
	SetPlayMode(InGamePlayMode::Play);
}

void Scene_InGame::SetPlayMode(InGamePlayMode mode)
{
	m_CurrPlayMode = mode;
	m_TileGrid->SetIsVisible(true);

	switch (m_CurrPlayMode)
	{
	case InGamePlayMode::Play:
	{
		INGAMETIME->Play();
		m_Player->SetIsActive(true);
		break;
	}
	case InGamePlayMode::Stop:
		INGAMETIME->Stop();
		m_Player->SetIsActive(false);
		break;
	case InGamePlayMode::Debug:
		INGAMETIME->Stop();
		m_Player->SetIsActive(false);
		break;
	}
}


void Scene_InGame::UpdatePlayMode(float dt)
{
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::Num1))
	{
		SetPlayMode(InGamePlayMode::Play);
	}
	else if (INPUT_MGR->GetKeyDown(sf::Keyboard::Num2))
	{
		SetPlayMode(InGamePlayMode::Debug);
	}

	switch (m_CurrPlayMode)
	{
	case InGamePlayMode::Play:
		UpdatePlay(dt);
		break;
	case InGamePlayMode::Stop:
		UpdateStop(dt);
		break;
	case InGamePlayMode::Debug:
		UpdateDebug(dt);
		break;
	}
}

void Scene_InGame::UpdatePlay(float dt)
{
	if (m_Player != nullptr)
	{
		GAME_MGR->SetViewCenter(0, m_Player->getPosition());
		GAME_MGR->SetViewCenter(1, m_Player->getPosition());
		GAME_MGR->SetViewCenter(2, m_Player->getPosition());
	}

	if (INPUT_MGR->GetKey(sf::Keyboard::Num9))
	{
		m_TileObjectSystem->ColorizePassableTile();
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::Num0))
	{
		m_TileObjectSystem->ColorizeInteractiveTile();
	}
}

void Scene_InGame::UpdateStop(float dt)
{
}

void Scene_InGame::UpdateDebug(float dt)
{
	DebugInputUpdate(dt);
}

void Scene_InGame::DebugInputUpdate(float dt)
{
	CellIndex mousetile = m_TileObjectSystem->GetTileCoordinatedTileIndex(INPUT_MGR->GetMouseViewPos(0));
	std::cout << mousetile.x << "," << mousetile.y << std::endl;

	m_TileGrid->SetIsVisible(true);

	if (INPUT_MGR->GetKey(sf::Keyboard::Num0))
	{
		m_TileObjectSystem->ColorizePassableTile();
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::Num9))
	{
		m_TileObjectSystem->ColorizeInteractiveTile();
	}
	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		sf::Vector2f mousepos = INPUT_MGR->GetMouseViewPos(0);

		GAME_MGR->SetViewCenter(0, mousepos);
		GAME_MGR->SetViewCenter(1, mousepos);
		GAME_MGR->SetViewCenter(2, mousepos);
	}
}
