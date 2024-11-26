#include "pch.h"
#include "Scene_Slice.h"
#include "TargetRect.h"
#include "rapidcsv.h"

Scene_Slice::Scene_Slice()
	:SceneBase("Slice", 2, 2)
{
}

Scene_Slice::~Scene_Slice()
{
}

bool Scene_Slice::Initialize()
{
	SetViewNeedPriority(0, false);
	m_Target = AddGameObject(0, new SpriteObject(""));
	m_TargetRect = AddGameObject(0, new TargetRect());
	return false;
}

void Scene_Slice::Reset()
{
}

void Scene_Slice::Enter()
{
	ImGuiManager::SetShowDemo(true);
	FRAMEWORK->SetBackColor(sf::Color::White);
	m_TargetSprite = dynamic_cast<DSprite*>(m_Target->GetDrawableObj(0));
	m_TargetRectangle = dynamic_cast<DRectangle*>(m_TargetRect->GetDrawableObj(0));
}

void Scene_Slice::Update(float dt)
{
	if (!ImGuiManager::IsImGuiWindowHasFocus())
	{
		if (INPUT_MGR->GetKeyDown(sf::Keyboard::LBracket))
		{
			GAME_MGR->SetViewZoom(0, 0.5f);
		}
		else if (INPUT_MGR->GetKeyDown(sf::Keyboard::RBracket))
		{
			GAME_MGR->SetViewZoom(0, 2.0f);
		}

		GAME_MGR->MoveView(0, { 0, -INPUT_MGR->GetAxisRaw(Axis::Vertical) * 5 });
		GAME_MGR->MoveView(0, { INPUT_MGR->GetAxisRaw(Axis::Horizontal) * 5,0 });

		if (m_IsSlicingNow)
		{
			if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left))
			{
				m_TargetRect->SetRect(GAME_MGR->GetScreenToViewPos(0, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left)),
					INPUT_MGR->GetMouseViewPos(0));

				m_Rect[0] = GAME_MGR->GetScreenToViewPos(0, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left)).x;
				m_Rect[1] = GAME_MGR->GetScreenToViewPos(0, INPUT_MGR->GetPrevMouseDown(sf::Mouse::Left)).y;
				m_Rect[2] = INPUT_MGR->GetMouseViewPos(0).x - m_Rect[0];
				m_Rect[3] = INPUT_MGR->GetMouseViewPos(0).y - m_Rect[1];
			}
		}

		if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
		{
			m_IsSlicingNow = false;
		}
	}

	m_TargetRect->SetRect(sf::FloatRect(m_Rect[0], m_Rect[1], m_Rect[2], m_Rect[3]));

}

void Scene_Slice::ShowSceneImgui()
{
	ImGui::Begin("Slicer");
	ImGui::Text("FilePath");
	ImGui::Text("target\\");
	ImGui::SameLine();
	ImGui::InputText("##FilePathString", m_FilePathBuff, IM_ARRAYSIZE(m_FilePathBuff));

	if (ImGui::Button("Sprtie Load"))
	{
		m_CurrentFilePath = m_FilePathBuff;
		m_FilePathBuff[0] = '\0';
		m_TargetSprite->SetTexture("target\\" + m_CurrentFilePath, true);
	}
	ImGui::Text("Current file : %s", m_CurrentFilePath);

	ImGui::Text("Sprite Clip Id");
	if (ImGui::InputText("##IdString", m_IdBuff, IM_ARRAYSIZE(m_IdBuff)))
	{
		m_CurrentId = m_IdBuff;
	}
	ImGui::Text("Current Id : %s", m_CurrentId);

	ImGui::Checkbox("Slice", &m_IsSlicingNow);

	ImGui::InputFloat("X##tl", &m_Rect[0]);
	ImGui::SameLine();
	if (ImGui::Button("-##tlx"))
	{
		m_Rect[0]--;
	}
	ImGui::SameLine();
	if (ImGui::Button("+##tlx"))
	{
		m_Rect[0]++;
	}

	ImGui::InputFloat("Y##tl", &m_Rect[1]);
	ImGui::SameLine();
	if (ImGui::Button("-##tly"))
	{
		m_Rect[1]--;
	}
	ImGui::SameLine();
	if (ImGui::Button("+##tly"))
	{
		m_Rect[1]++;
	}

	ImGui::InputFloat("W##width", &m_Rect[2]);
	ImGui::SameLine();
	if (ImGui::Button("-##width"))
	{
		m_Rect[2]--;
	}
	ImGui::SameLine();
	if (ImGui::Button("+##width"))
	{
		m_Rect[2]++;
	}

	ImGui::InputFloat("H##height", &m_Rect[3]);
	ImGui::SameLine();
	if (ImGui::Button("-##height"))
	{
		m_Rect[3]--;
	}
	ImGui::SameLine();
	if (ImGui::Button("+##height"))
	{
		m_Rect[3]++;
	}

	if (ImGui::Button("Save Texcoord"))
	{
		Save(m_CurrentId, m_CurrentFilePath, m_Rect);
	}


	if (ImGui::ColorEdit3("Color", m_Color))
	{
		FRAMEWORK->SetBackColor(sf::Color(m_Color[0] * 255, m_Color[1] * 255, m_Color[2] * 255));
	}
	ImGui::SameLine();

	ImGui::End();
}

void Scene_Slice::Save(const std::string& id, const std::string& filepath, float rect[])
{

	rapidcsv::Document doc("filePath");
	doc.RemoveRow(id);

	// 새로운 행 추가
	std::vector<std::string> newRow;
	newRow.push_back(id);
	newRow.push_back(filepath);
	for (int i = 0; i < 4; i++)
		newRow.push_back(std::to_string(rect[i]));
	doc.InsertRow(doc.GetRowCount(), newRow);

	// 수정된 데이터를 저장
	doc.Save("filePath");
}
