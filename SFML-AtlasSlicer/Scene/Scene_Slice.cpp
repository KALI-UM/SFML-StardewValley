#include "pch.h"
#include "Scene_Slice.h"
#include "TargetRect.h"
#include "TileGrid.h"
#include "rapidcsv.h"
#include <direct.h>

Scene_Slice::Scene_Slice()
	:SceneBase("Slice", 2, 2)
{
}

Scene_Slice::~Scene_Slice()
{
}

bool Scene_Slice::Initialize()
{
	rapidcsv::Document doc("output/AtlasSlicer.csv", rapidcsv::LabelParams(-1, -1));
	m_Unit = doc.GetCell<int>(1,0);

	SetViewNeedPriority(0, false);
	m_Target = AddGameObject(0, new SpriteObject("keyboard.png"));
	m_TargetRect = AddGameObject(0, new TargetRect());
	m_Grid = AddGameObject(0, new TileGrid());
	m_Grid->SetCellCount({ 1000, 1000 });
	m_Grid->SetCellSize({ (float)m_Unit,(float)m_Unit });

	auto guide = AddGameObject(m_UILayerIndex, new SpriteObject("keyboard.png"));
	guide->setScale(0.5, 0.5);
	guide->setPosition(0, GAME_MGR->GetWindow()->getSize().y * 0.8f);

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
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::LBracket))
	{
		GAME_MGR->SetViewZoom(0, 0.5f);
	}
	else if (INPUT_MGR->GetKeyDown(sf::Keyboard::RBracket))
	{
		GAME_MGR->SetViewZoom(0, 2.0f);
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Right))
	{
		GAME_MGR->SetViewCenter(0, INPUT_MGR->GetMouseViewPos(0));
	}

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

		if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
		{
			m_IsSlicingNow = false;
		}
	}

	if (INPUT_MGR->GetKey(sf::Keyboard::F1))
	{
		m_Rect[0]--;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F2))
	{
		m_Rect[0]++;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F3))
	{
		m_Rect[1]--;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F4))
	{
		m_Rect[1]++;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F5))
	{
		m_Rect[2]--;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F6))
	{
		m_Rect[2]++;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F7))
	{
		m_Rect[3]--;
	}
	if (INPUT_MGR->GetKey(sf::Keyboard::F8))
	{
		m_Rect[3]++;
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::G))
	{
		m_Grid->SetIsVisible(!m_Grid->GetIsVisible());
	}

	m_TargetRect->SetRect(sf::FloatRect(m_Rect[0], m_Rect[1], m_Rect[2], m_Rect[3]));
}

void Scene_Slice::ShowSceneImgui()
{
	ImGuiManager::SetImGuiWindowHasFocus(false);

	ImGui::Begin("Slicer");
	ImGui::Text("FilePath");
	ImGui::Text("target\\");
	ImGui::SameLine();
	if (ImGui::InputText("##FilePathString", m_FilePathBuff, IM_ARRAYSIZE(m_FilePathBuff)))
	{
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}

	if (ImGui::Button("Sprtie Load"))
	{
		m_CurrentFilePath = m_FilePathBuff;
		m_FilePathBuff[0] = '\0';
		m_TargetSprite->SetTexture("target\\" + m_CurrentFilePath, true);
	}
	ImGui::Text("Current file : %s", m_CurrentFilePath.c_str());

	ImGui::Text("Sprite Clip Id");
	if (ImGui::InputText("##IdString", m_IdBuff, IM_ARRAYSIZE(m_IdBuff)))
	{
		m_CurrentId = m_IdBuff;
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}
	ImGui::Text("Current Id : %s", m_CurrentId.c_str());

	ImGui::Checkbox("Slice", &m_IsSlicingNow);
	ImGui::SameLine();
	ImGui::Text("X : %d", m_SliceXCount);
	ImGui::SameLine();
	if (ImGui::Button("-##XCount"))
	{
		m_SliceXCount--;
		m_SliceXCount = Utils::Clamp(m_SliceXCount, 1, 10000);
		m_TargetRect->SliceRect(m_SliceXCount, m_SliceYCount);
	}
	ImGui::SameLine();
	if (ImGui::Button("+##XCount"))
	{
		m_SliceXCount++;
		m_SliceXCount = Utils::Clamp(m_SliceXCount, 1, 10000);
		m_TargetRect->SliceRect(m_SliceXCount, m_SliceYCount);
	}
	ImGui::SameLine();
	ImGui::Text("Y : %d", m_SliceYCount);
	ImGui::SameLine();
	if (ImGui::Button("-##YCount"))
	{
		m_SliceYCount--;
		m_SliceYCount = Utils::Clamp(m_SliceYCount, 1, 1000);
		m_TargetRect->SliceRect(m_SliceXCount, m_SliceYCount);
	}
	ImGui::SameLine();
	if (ImGui::Button("+##YCount"))
	{
		m_SliceYCount++;
		m_SliceYCount = Utils::Clamp(m_SliceYCount, 1, 1000);
		m_TargetRect->SliceRect(m_SliceXCount, m_SliceYCount);
	}

	if (ImGui::InputFloat("X##tl", &m_Rect[0]))
	{
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##tlx"))
	{
		m_Rect[0]--;
		m_Rect[0] = Utils::Clamp(m_Rect[0], (float)0, (float)m_TargetSprite->GetTextureSize().x);
	}
	ImGui::SameLine();
	if (ImGui::Button("+##tlx"))
	{
		m_Rect[0]++;
		m_Rect[0] = Utils::Clamp(m_Rect[0], (float)0, (float)m_TargetSprite->GetTextureSize().x);
	}

	if (ImGui::InputFloat("Y##tl", &m_Rect[1]))
	{
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##tly"))
	{
		m_Rect[1]--;
		m_Rect[1] = Utils::Clamp(m_Rect[1], (float)0, (float)m_TargetSprite->GetTextureSize().y);
	}
	ImGui::SameLine();
	if (ImGui::Button("+##tly"))
	{
		m_Rect[1]++;
		m_Rect[1] = Utils::Clamp(m_Rect[1], (float)0, (float)m_TargetSprite->GetTextureSize().y);

	}

	if (ImGui::InputFloat("W##width", &m_Rect[2]))
	{
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##width"))
	{
		m_Rect[2]--;
		m_Rect[2] = Utils::Clamp(m_Rect[2], (float)0, (float)m_TargetSprite->GetTextureSize().x);

	}
	ImGui::SameLine();
	if (ImGui::Button("+##width"))
	{
		m_Rect[2]++;
		m_Rect[2] = Utils::Clamp(m_Rect[2], (float)0, (float)m_TargetSprite->GetTextureSize().x);
	}

	if (ImGui::InputFloat("H##height", &m_Rect[3]))
	{
		ImGuiManager::SetImGuiWindowHasFocus(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("-##height"))
	{
		m_Rect[3]--;
		m_Rect[3] = Utils::Clamp(m_Rect[3], (float)0, (float)m_TargetSprite->GetTextureSize().y);
	}
	ImGui::SameLine();
	if (ImGui::Button("+##height"))
	{
		m_Rect[3]++;
		m_Rect[3] = Utils::Clamp(m_Rect[3], (float)0, (float)m_TargetSprite->GetTextureSize().y);
	}

	ImGui::NewLine();
	if (ImGui::Button("Save Texcoord"))
	{
		Save(m_CurrentId, m_CurrentFilePath, m_Rect);
	}

	ImGui::Text("BackColor");
	if (ImGui::ColorEdit3("Color", m_Color))
	{
		FRAMEWORK->SetBackColor(sf::Color(m_Color[0] * 255, m_Color[1] * 255, m_Color[2] * 255));
	}
	ImGui::SameLine();

	ImGui::End();
}

void Scene_Slice::Save(const std::string& id, const std::string& filename, float rect[])
{
	rapidcsv::Document doc("output/AtlasSlicer.csv", rapidcsv::LabelParams(1, -1));
	std::string slicerId = filename + "#" + id;
	std::vector<std::string> temp = doc.GetColumn<std::string>(0);
	auto it = std::find(temp.begin(), temp.end(), slicerId);
	if (it != temp.end())
	{
		std::cout << "Remove Prev data[" << slicerId << "...\n";
		doc.RemoveRow(it - temp.begin());
	}
	// 새로운 행 추가
	std::vector<std::string> newRow;
	newRow.push_back(slicerId);
	newRow.push_back(filename);
	newRow.push_back(std::to_string((int)rect[2] / m_Unit) + "x" + std::to_string((int)rect[3] / m_Unit));
	newRow.push_back("0,0");
	for (int i = 0; i < 4; i++)
		newRow.push_back(std::to_string((int)rect[i]));
	doc.InsertRow(doc.GetRowCount(), newRow);

	std::cout << "Save : ";
	for (int i = 0; i < newRow.size() - 1; i++)
		std::cout << newRow[i] << ",";
	std::cout << newRow[newRow.size() - 1] << std::endl;

	sf::Image originalImage = TEXTURE_MGR->GetByFilepath("target/" + m_CurrentFilePath)->copyToImage();
	SaveAsPng("", slicerId, sf::IntRect(rect[0], rect[1], rect[2], rect[3]), originalImage);

	//여러장이었을 때
	if (m_SliceXCount != 1 || m_SliceYCount != 1)
	{
		std::string filepath = "output/" + slicerId;

		int nResult = mkdir(filepath.c_str());
		if (nResult == 0)
		{
			std::cout << "folder : " + filepath <<" - mkdir" << std::endl;
		}
		else if (nResult == -1)
		{
			std::cout << "folder : " + filepath << " - exist" << std::endl;
		}

		temp.clear();
		for (int j = 0; j < m_SliceYCount; j++)
		{
			for (int i = 0; i < m_SliceXCount; i++)
			{
				std::vector<std::string> newRowChild;
				std::string childId = slicerId + "_X" + std::to_string(i) + "Y" + std::to_string(j);
				temp = doc.GetColumn<std::string>(0);
				auto it2 = std::find(temp.begin(), temp.end(), childId);
				if (it2 != temp.end())
				{
					std::cout << "Remove Prev data[" << childId << "]...\n";
					doc.RemoveRow(it2 - temp.begin());
				}

				newRowChild.push_back(childId);
				newRowChild.push_back(filename);

				const auto& subrect = m_TargetRect->GetSubRect(i, j);
				newRowChild.push_back(std::to_string((int)subrect.width / m_Unit) + "x" + std::to_string((int)subrect.height / m_Unit));
				newRowChild.push_back(std::to_string(i) + "," + std::to_string(j));
				newRowChild.push_back(std::to_string((int)subrect.left));
				newRowChild.push_back(std::to_string((int)subrect.top));
				newRowChild.push_back(std::to_string((int)subrect.width));
				newRowChild.push_back(std::to_string((int)subrect.height));
				doc.InsertRow(doc.GetRowCount(), newRowChild);

				std::cout << "Save : ";
				for (int i = 0; i < newRowChild.size() - 1; i++)
					std::cout << newRowChild[i] << ",";
				std::cout << newRowChild[newRowChild.size() - 1] << std::endl;

				SaveAsPng(slicerId, childId, sf::IntRect(std::stoi(newRowChild[4]), std::stoi(newRowChild[5]), std::stoi(newRowChild[6]), std::stoi(newRowChild[7])), originalImage);
			}
		}
	}

	// 수정된 데이터를 저장
	doc.Save("output/AtlasSlicer.csv");
}

void Scene_Slice::SaveAsPng(const std::string filepath, const std::string& id, const sf::IntRect& rect, const sf::Image& image)
{
	sf::IntRect cropArea = rect;

	// 텍스처의 잘라낼 영역을 Image로 추출
	//sf::Image originalImage = TEXTURE_MGR->GetByFilepath("target/" + m_CurrentFilePath)->copyToImage();
	sf::Image croppedImage;

	// 잘라낸 영역만큼 새로운 이미지를 생성
	croppedImage.create(cropArea.width, cropArea.height);
	croppedImage.copy(image, 0, 0, cropArea);

	// 잘라낸 이미지를 PNG로 저장
	if (!croppedImage.saveToFile("output/"+filepath+"/" + id + ".png"))
	{
		std::cerr << "PNG Fail!" << std::endl;
	}
}
