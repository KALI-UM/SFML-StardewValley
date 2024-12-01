#include "pch.h"
#include "Scene_TileEditor.h"

#include "Tile/TileGrid.h"
#include "Tile/TileMapSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/TileViewChild.h"
#include "Tile/TileController.h"
#include "Tile/ButtonBar.h"


Scene_TileEditor::Scene_TileEditor()
	:SceneBase("TileEditor", 4, 2)
{
}

Scene_TileEditor::~Scene_TileEditor()
{
}

bool Scene_TileEditor::Initialize()
{
	SetLayerViewIndex(0, 0);
	SetLayerViewIndex(1, 0);
	SetLayerViewIndex(2, 0);
	SetLayerViewIndex(3, 0);
	SetViewNeedPriority(0, false);

	m_TileModel = AddGameObject(0, new TileModel({ 80,64 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileLayerView(TileViewLayer::Terrain, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileViewLayer::Back, AddGameObject(0, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileViewLayer::Buildings, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileViewLayer::Paths, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileViewLayer::Front, AddGameObject(1, new TileViewChild(m_TileView)));
	m_TileView->SetTileLayerView(TileViewLayer::AlwaysFront, AddGameObject(2, new TileViewChild(m_TileView)));
	m_TileMapSystem = AddGameObject(m_UILayerIndex, new TileMapSystem(m_TileModel));
	m_TileController = AddGameObject(m_UILayerIndex, new TileController(m_TileMapSystem, m_TileModel, m_TileView, 0));

	m_TileGrid = AddGameObject(3, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);

	m_ButtonBar = AddGameObject(m_UILayerIndex, new ButtonBar(m_UIViewIndex));
	m_TileController->SetButtonBar(m_ButtonBar);

	m_MiniMap = AddGameObject(m_UILayerIndex, new SpriteObject());

	for (int i = 0; i < (int)TileViewLayer::Max; i++)
		m_Layers.push_back(Tile::TileViewLayerToString((TileViewLayer)i));

	
	return false;
}

void Scene_TileEditor::Enter()
{
	ImGuiManager::SetShowDemo(true);
	FRAMEWORK->SetBackColor(ColorPalette::White);
	m_MiniMapTexture.create(GAME_MGR->GetView(0)->getSize().x, GAME_MGR->GetView(0)->getSize().y);
	m_MiniMap->SetTexture(&m_MiniMapTexture.getTexture());
	
	m_MiniMap->setScale(0.3f, -0.3f);
	m_MiniMap->setPosition(10, 1000);
	
	m_ButtonBar->setScale(2, 2);
}

void Scene_TileEditor::Update(float dt)
{
}

void Scene_TileEditor::PostRender()
{
	m_TileGrid->SetIsVisible(false);
	RenderViewToRenderTexture(0, m_MiniMapTexture);
	m_TileGrid->SetIsVisible(true);
}

void Scene_TileEditor::ShowSceneImgui()
{
	ViewLayerImgui();
	CollLayerImgui();
}

void Scene_TileEditor::ViewLayerImgui()
{
	static int currIndex = 0;
	ImGui::Begin("View Layer Window");

	if (ImGui::Button("Place"))
	{
		m_TileController->SetControlStatus(ControlStatus::Place);
	}
	ImGui::SameLine();
	if (ImGui::Button("Eraser"))
	{
		m_TileController->SetControlStatus(ControlStatus::Destroy);
	}

	if (ImGui::BeginCombo("ViewLayer", m_Layers[currIndex].c_str()))
	{
		for (int i = 0; i < m_Layers.size(); i++) {
			bool isSelected = (currIndex == i);
			if (ImGui::Selectable(m_Layers[i].c_str(), isSelected)) {
				currIndex = i; // 선택 변경
				m_TileMapSystem->SetCurrTileLayer((TileViewLayer)i);
			}

			// 선택된 항목 강조 표시
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::Button("Save"))
	{
		m_TileMapSystem->SaveTileViewLayerFile((TileViewLayer)currIndex);
	}

	static bool visiblelayer[(int)TileViewLayer::Max] = { true, true,true,true,true,true };
	if (ImGui::Checkbox(m_Layers[0].c_str(), &visiblelayer[0]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)0, visiblelayer[0]);
	}
	if (ImGui::Checkbox(m_Layers[1].c_str(), &visiblelayer[1]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)1, visiblelayer[1]);
	}
	if (ImGui::Checkbox(m_Layers[2].c_str(), &visiblelayer[2]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)2, visiblelayer[2]);
	}
	if (ImGui::Checkbox(m_Layers[3].c_str(), &visiblelayer[3]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)3, visiblelayer[3]);
	}
	if (ImGui::Checkbox(m_Layers[4].c_str(), &visiblelayer[4]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)4, visiblelayer[4]);
	}
	if (ImGui::Checkbox(m_Layers[5].c_str(), &visiblelayer[5]))
	{
		m_TileView->SetTileLayerVisible((TileViewLayer)5, visiblelayer[5]);
	}
	ImGui::End();
}

void Scene_TileEditor::CollLayerImgui()
{
	static int currIndex = 0;
	ImGui::Begin("Collision Layer Window");
	if (ImGui::Button("Place"))
	{
		m_TileController->SetControlStatus(ControlStatus::Collision);
		m_TileController->m_CurrCollType = (CollisionType)currIndex;
	}
	ImGui::SameLine();
	if (ImGui::Button("BackToTileView"))
	{
		m_TileController->SetControlStatus(ControlStatus::Place);
	}

	static std::vector<std::string> collisionType = {"passable", "block"};
	if (ImGui::BeginCombo("Collision Type", collisionType[currIndex].c_str()))
	{
		for (int i = 0; i < collisionType.size(); i++) {
			bool isSelected = (currIndex == i);
			if (ImGui::Selectable(collisionType[i].c_str(), isSelected)) {
				currIndex = i; // 선택 변경
				m_TileController->m_CurrCollType = (CollisionType)currIndex;
			}

			// 선택된 항목 강조 표시
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("Save"))
	{
		m_TileMapSystem->SaveTileCollisionLayerFile();
	}

	ImGui::End();
}
