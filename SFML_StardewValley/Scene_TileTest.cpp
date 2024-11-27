#include "pch.h"
#include "Scene_TileTest.h"
#include "TileGrid.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileViewChild.h"


Scene_TileTest::Scene_TileTest()
    :SceneBase("TileTest", 3,3)
{
}

Scene_TileTest::~Scene_TileTest()
{
}

bool Scene_TileTest::Initialize()
{
	m_TileGrid = AddGameObject(1, new TileGrid());
	m_TileGrid->SetIsVisible(false);
	m_TileGrid->SetCellCount({ 128,128 });
	m_TileGrid->SetCellSize({ 16,16 });

	m_TileModel = AddGameObject(0, new TileModel({ 128,128 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileLayerView(TileLayer::Back, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Buildings, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Paths, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Front, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::AlwaysFront, AddGameObject(2, new TileViewChild(m_TileView)));

    return false;
}

void Scene_TileTest::Enter()
{
}

void Scene_TileTest::Update(float dt)
{
}
