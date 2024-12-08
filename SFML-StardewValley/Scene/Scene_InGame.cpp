#include "pch.h"
#include "Scene_InGame.h"
#include "InGameTime.h"
#include "InGameEvent.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileGrid.h"

#include "Tile/TileObject.h"
#include "InGamePlayer.h"

#include "Inventory.h"
#include "InventoryUI.h"

#include "EventUI.h"
#include "GameInfoUI.h"

#include "ItemGenerator.h"
#include "Item/DropItem.h"

#include "Item/Tool.h"

Scene_InGame::Scene_InGame(const std::string& name)
	:SceneBase("InGame::" + name, (int)TileObjLayer::Max + 5, (int)ViewLayer::Max)
{
}

Scene_InGame::~Scene_InGame()
{
}

bool Scene_InGame::Initialize()
{
	SetLayerViewIndex((int)TileObjLayer::Back, (int)ViewLayer::Back);
	SetLayerViewIndex((int)TileObjLayer::Buildings, (int)ViewLayer::Object);
	SetLayerViewIndex((int)TileObjLayer::Paths, (int)ViewLayer::Object);
	SetLayerViewIndex((int)TileObjLayer::Front, (int)ViewLayer::Object);
	SetLayerViewIndex((int)TileObjLayer::AlwaysFront, (int)ViewLayer::Front);

	SetLayerViewIndex(5, (int)ViewLayer::Terrain);
	SetLayerViewIndex(6, (int)ViewLayer::WaterEffect);
	SetLayerViewIndex(7, (int)ViewLayer::Effect);
	SetLayerViewIndex(8, (int)ViewLayer::Debug);
	//레이어5 = terrain, 
	//레이어6 = watereffect
	//레이어7 = effect
	//레이어8 = debug



	SetViewNeedPriority(0, false);
	SetViewNeedPriority(2, false);
	//Terrain, WaterEffect, Back == 0 (SelfPriority)
	//Object, Player == 1
	//Front, Debug == 2 (SelfPriority)
	//effect == 3
	m_MapSize = { 16 * 90, 16 * 64 };

	m_TileModel = AddGameObject(0, new TileModel((unsigned int)ViewLayer::Max, { 90,64 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileViewIndex((int)ViewLayer::Terrain, AddGameObject(5, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)ViewLayer::WaterEffect, AddGameObject(6, new TileViewChild(m_TileView, TileViewType::TexId)));
	m_TileView->SetTileViewIndex((int)ViewLayer::Back, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)ViewLayer::Object, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)ViewLayer::Front, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Object)));
	m_TileView->SetTileViewIndex((int)ViewLayer::Effect, AddGameObject(7, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileView->SetTileViewIndex((int)ViewLayer::Debug, AddGameObject(8, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileObjectSystem = AddGameObject(m_UILayerIndex, new TileObjectSystem(m_TileModel, m_TileView, this));

	m_TileGrid = AddGameObject((int)TileObjLayer::Front, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);

	m_Player = AddGameObject((int)TileObjLayer::Paths, new InGamePlayer("Player"));

	m_Inventory = AddGameObject(m_UILayerIndex, new Inventory());
	m_InventoryUI = AddUIGameObject(m_UILayerIndex, new InventoryUI());
	m_Inventory->SetInventoryUI(m_InventoryUI);
	m_Player->SetInventory(m_Inventory);

	m_PopUpUI = AddUIGameObject(m_UILayerIndex, new EventUI());
	m_GameInfoUI = AddUIGameObject(m_UILayerIndex, new GameInfoUI());

	m_DropItems.Initialize(this, 30, ExpandOption::GetOldUsed, (int)TileObjLayer::Paths);

	return true;
}

void Scene_InGame::Enter()
{
	SetPlayMode(InGamePlayMode::Play);
	INGAMETIME->Enter(this);
	INGAMEEVENT->Enter(this);
	ITEMGENERATOR->Enter(this);

	m_TileObjectSystem->LoadTileLayerRawFile(m_TerrainFilepath);

	m_Player->SetTileSystem(m_TileObjectSystem);
	m_Player->setPosition(InGameEvent::m_EnterPoint.To<float>() * 16.0f + sf::Vector2f(8.0f, 8.0f));

}

void Scene_InGame::Update(float dt)
{
	UpdatePlayMode(dt);
	INGAMETIME->Update(dt);
	m_DropItems.Update(dt);
}

void Scene_InGame::Reset()
{
	m_DropItems.Reset();
}

void Scene_InGame::Exit()
{
	SetPlayMode(InGamePlayMode::Stop);
	//m_DropItems.Exit();
}

void Scene_InGame::OnWindowResize()
{
	for (int layer = 0; layer < (int)ViewLayer::Max; layer++)
		GAME_MGR->SetViewZoom(layer, 0.25f);

}

void Scene_InGame::SetPlayMode(InGamePlayMode mode)
{
	m_CurrPlayMode = mode;
	m_TileGrid->SetIsVisible(true);

	switch (m_CurrPlayMode)
	{
	case InGamePlayMode::Play:
	{
		m_TileObjectSystem->SetEffectLayerColor(ColorPalette::Black, ColorPalette::Transparent, 1.0f);
		INGAMETIME->Play();
		if (!SOUND_MGR->IsBgmPlaying())
		{
			if (!m_IsSunset)
				SOUND_MGR->PlayBgm("sounds/Ambience/spring_day.wav", false, true, 3, 20);
			else
				SOUND_MGR->PlayBgm("sounds/Ambience/spring_night.wav", false, true, 3, 20);
		}
		m_Player->SetIsActive(true);
		break;
	}
	case InGamePlayMode::Stop:
		INGAMETIME->Stop();
		m_Player->SetIsActive(false);
		break;

	case InGamePlayMode::PlayCutScene:
		m_CutSceneTimer = 0;
		INGAMETIME->Stop();
		m_TileObjectSystem->SetEffectLayerColor(m_IsSunset ? m_SunsetLightColor : ColorPalette::Transparent, ColorPalette::Black, 3.0f);
		m_Player->SetIsActive(true);
		break;

	case InGamePlayMode::PlayChangeScene:
		m_CutSceneTimer = 0;
		INGAMETIME->Stop();
		m_TileObjectSystem->SetEffectLayerColor(m_IsSunset ? m_SunsetLightColor : ColorPalette::Transparent, ColorPalette::Black, 3.0f);
		m_Player->SetIsActive(true);
		break;

	case InGamePlayMode::Debug:
		INGAMETIME->Stop();
		m_Player->SetIsActive(false);
		break;
	}
}

void Scene_InGame::EndDay()
{
	SOUND_MGR->StopBgm();
	if (m_IsPlayerInHouse)
	{
		SetPlayMode(InGamePlayMode::PlayCutScene);
		m_IsSunset = false;
		INGAMETIME->NewDay();
	}
	else
	{
		SetPlayMode(InGamePlayMode::PlayChangeScene);
		INGAMETIME->NewDay();
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
	case InGamePlayMode::PlayCutScene:
		UpdateCutScene(dt);
		break;
	case InGamePlayMode::PlayChangeScene:
		UpdateChangeScene(dt);
		break;
	case InGamePlayMode::Debug:
		UpdateDebug(dt);
		break;
	}
}

void Scene_InGame::UpdatePlay(float dt)
{
	if (!m_IsSunset && INGAMETIME->IsSunsetTimePassed())
	{
		m_TileObjectSystem->SetEffectLayerColor(ColorPalette::Transparent, m_SunsetLightColor, 3.0f);
		SOUND_MGR->PlayBgm("sounds/Ambience/spring_night.wav", false, true, 3, 20);
		m_IsSunset = true;
	}


	if (INGAMETIME->GetPassedDayTimeRatio() >= 1.0f)
	{
		EndDay();
	}

	if (m_Player != nullptr)
	{
		sf::Vector2f viewsize = GAME_MGR->GetViewSize(0);

		sf::Vector2f viewCenter = { Utils::Clamp(m_Player->getPosition().x ,viewsize.x / 2, m_MapSize.x - viewsize.x / 2),
				Utils::Clamp(m_Player->getPosition().y, viewsize.y / 2, m_TileModel->m_CellCount.y * m_TileModel->m_CellSize.y - viewsize.y / 2) };

		if (viewsize.x >= m_MapSize.x)
			viewCenter.x = m_MapSize.x / 2;

		if (viewsize.y >= m_MapSize.y)
			viewCenter.y = m_MapSize.y / 2;

		for (int layer = 0; layer < (int)ViewLayer::Max; layer++)
			GAME_MGR->SetViewCenter(layer, viewCenter);
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

void Scene_InGame::UpdateCutScene(float dt)
{
	m_CutSceneTimer += dt;

	if (m_CutSceneTimer >= 3.0f)
	{
		SetPlayMode(InGamePlayMode::Play);
		return;
	}
}

void Scene_InGame::UpdateChangeScene(float dt)
{
	m_CutSceneTimer += dt;

	if (m_CutSceneTimer >= 3.0f)
	{
		INGAMEEVENT->ENTER("3,10#FarmHouseIndoor");
		return;
	}
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

		for (int layer = 0; layer < (int)ViewLayer::Max; layer++)
			GAME_MGR->SetViewCenter(layer, mousepos);
	}
}
