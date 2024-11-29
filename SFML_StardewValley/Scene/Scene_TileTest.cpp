#include "pch.h"
#include "Scene_TileTest.h"

#include "Tile/TileGrid.h"
#include "Tile/TileMapSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileController.h"
#include "Tile/ButtonBar.h"


Scene_TileTest::Scene_TileTest()
	:SceneBase("TileTest", 4, 2)
{
}

Scene_TileTest::~Scene_TileTest()
{
}

bool Scene_TileTest::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 0);
	SetLayerViewIndex(2, 0);
	SetLayerViewIndex(3, 0);
	SetViewNeedPriority(0, false);

	m_TileModel = AddGameObject(0, new TileModel({ 128,128 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileLayerView(TileLayer::Back, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Buildings, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Paths, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::Front, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileLayer::AlwaysFront, AddGameObject(2, new TileViewChild(m_TileView)));
	m_TileMapSystem = AddGameObject(m_UILayerIndex, new TileMapSystem(m_TileModel));
	m_TileController = AddGameObject(m_UILayerIndex, new TileController(m_TileMapSystem, m_TileModel, m_TileView, 0));

	m_TileGrid = AddGameObject(3, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);

	m_ButtonBar = AddGameObject(m_UILayerIndex, new ButtonBar(m_UIViewIndex));
	m_TileController->SetButtonBar(m_ButtonBar);

	for (int i = 0; i < (int)TileLayer::Max; i++)
		m_Layers.push_back(Tile::TileLayerToString((TileLayer)i));

	return false;
}

void Scene_TileTest::Enter()
{
	ImGuiManager::SetShowDemo(true);
	FRAMEWORK->SetBackColor(ColorPalette::White);
	/*GAME_MGR->SetViewZoom(m_UIViewIndex, 0.8f);
	GAME_MGR->SetViewCenter(m_UIViewIndex, {300,300 });*/
}

void Scene_TileTest::Update(float dt)
{
}

void Scene_TileTest::ShowSceneImgui()
{
	ImGui::Begin("Layer Window");
	static int currIndex = 0;
	if (ImGui::BeginCombo("Layer", m_Layers[currIndex].c_str()))
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			bool isSelected = (currIndex == i);
			if (ImGui::Selectable(m_Layers[i].c_str(), isSelected)) {
				currIndex = i; // 선택 변경
				m_TileMapSystem->SetCurrTileLayer((TileLayer)i);
			}

			// 선택된 항목 강조 표시
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	static bool visiblelayer[(int)TileLayer::Max] = { true, true,true,true,true };
	if (ImGui::Checkbox(m_Layers[0].c_str(), &visiblelayer[0]))
	{
		m_TileView->SetTileLayerVisible((TileLayer)1, visiblelayer[0]);
	}
	if (ImGui::Checkbox(m_Layers[1].c_str(), &visiblelayer[1]))
	{
		m_TileView->SetTileLayerVisible((TileLayer)1, visiblelayer[1]);
	}
	if (ImGui::Checkbox(m_Layers[2].c_str(), &visiblelayer[2]))
	{
		m_TileView->SetTileLayerVisible((TileLayer)2, visiblelayer[2]);
	}
	if (ImGui::Checkbox(m_Layers[3].c_str(), &visiblelayer[3]))
	{
		m_TileView->SetTileLayerVisible((TileLayer)3, visiblelayer[3]);
	}
	if (ImGui::Checkbox(m_Layers[4].c_str(), &visiblelayer[4]))
	{
		m_TileView->SetTileLayerVisible((TileLayer)4, visiblelayer[4]);
	}

	ImGui::End();
}
