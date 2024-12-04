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

	m_TileModel = AddGameObject(0, new TileModel(1, { 3,6 }, { 16,16 }));
	m_TileView = AddGameObject(0, new TileView(m_TileModel));
	m_TileView->SetTileViewIndex((int)TileEditorLayer::Layer0, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Raw)));
	//m_TileView->SetTileViewIndex((int)TileEditorLayer::Layer1, AddGameObject(0, new TileViewChild(m_TileView, TileViewType::Raw)));
	m_TileMapSystem = AddGameObject(m_UILayerIndex, new TileMapSystem(m_TileModel));
	m_TileController = AddGameObject(m_UILayerIndex, new TileController(m_TileMapSystem, m_TileModel, m_TileView, 0));

	m_TileGrid = AddGameObject(3, new TileGrid());
	m_TileView->SetTileGrid(m_TileGrid);

	m_ButtonBar = AddGameObject(m_UILayerIndex, new ButtonBar(m_UIViewIndex));
	m_TileController->SetButtonBar(m_ButtonBar);

	m_MiniMap = AddGameObject(m_UILayerIndex, new SpriteObject());

	for (int i = 0; i < (int)TileLayer::Max; i++)
		m_Layers.push_back(Tile::TileLayerToString((TileLayer)i));

	for (int i = 0; i < (int)TileType::None; i++)
		m_Types.push_back(Tile::TileTypeToString((TileType)i));

	return false;
}

void Scene_TileEditor::Enter()
{
	ImGuiManager::SetShowDemo(true);
	FRAMEWORK->SetBackColor(ColorPalette::White);

	m_TileController->SetControlStatus(ControlStatus::Place);

	m_MiniMapTexture.create(16 * 80, 16 * 64);
	m_MiniMap->SetTexture(&m_MiniMapTexture.getTexture());

	m_MiniMap->setScale(0.3f, -0.3f);
	m_MiniMap->setPosition(300, 300);

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
	ImGui::Begin("Obj");

	static std::string objname;
	static char buff1[1000];
	ImGui::InputText("TOBJID", buff1, IM_ARRAYSIZE(buff1));
	m_Obj = buff1;

	if (ImGui::Button("Save"))
	{
		std::string texfilepath = "datatables/" + m_Obj + "tex.csv";
		std::string typefilepath = "datatables/" + m_Obj + "type.csv";
		m_TileMapSystem->SaveAsTileObjData("Back", texfilepath, typefilepath);
	}
	ImGui::End();

	ViewLayerImgui();
	TileTypeImgui();
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

	//if (ImGui::BeginCombo("ViewLayer", m_Layers[currIndex].c_str()))
	//{
	//	for (int i = 0; i < m_Layers.size(); i++) {
	//		bool isSelected = (currIndex == i);
	//		if (ImGui::Selectable(m_Layers[i].c_str(), isSelected)) {
	//			currIndex = i; // 선택 변경
	//			m_TileMapSystem->SetCurrTileLayer((TileEditorLayer)i);
	//		}

	//		// 선택된 항목 강조 표시
	//		if (isSelected) {
	//			ImGui::SetItemDefaultFocus();
	//		}
	//	}
	//	ImGui::EndCombo();
	//}

	if (ImGui::Button("Save"))
	{
		m_TileMapSystem->SaveTileViewRawFile((TileEditorLayer)currIndex, m_Obj);
	}

	static bool visiblelayer[2] = { true, true };
	if (ImGui::Checkbox(m_Layers[0].c_str(), &visiblelayer[0]))
	{
		m_TileView->SetTileViewVisible(0, visiblelayer[0]);
	}
	//if (ImGui::Checkbox(m_Layers[1].c_str(), &visiblelayer[1]))
	//{
	//	m_TileView->SetTileViewVisible(1, visiblelayer[1]);
	//}

	ImGui::End();
}

void Scene_TileEditor::TileTypeImgui()
{
	static int currIndex = 0;
	ImGui::Begin("TileType Layer Window");
	if (ImGui::Button("Place"))
	{
		m_TileController->SetControlStatus(ControlStatus::TileType);
		m_TileController->m_CurrTileType = (TileType)currIndex;
	}
	ImGui::SameLine();
	if (ImGui::Button("BackToTileView"))
	{
		m_TileController->SetControlStatus(ControlStatus::Place);
	}

	if (ImGui::BeginCombo("Collision Type", m_Types[currIndex].c_str()))
	{
		for (int i = 0; i < m_Types.size(); i++) {
			bool isSelected = (currIndex == i);
			if (ImGui::Selectable(m_Types[i].c_str(), isSelected)) {
				currIndex = i; // 선택 변경
				m_TileController->m_CurrTileType = (TileType)currIndex;
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
		m_TileMapSystem->SaveTileTypeFile(m_Obj);
	}

	ImGui::End();
}
