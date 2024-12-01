#include "pch.h"
#include "Tile/ButtonBar.h"
#include "D9SliceSprite.h"
#include "DTile.h"
#include "TexCoordTable.h"

ButtonBar::ButtonBar(int viewIndex)
	:m_ViewIndex(viewIndex)
{
}

ButtonBar::~ButtonBar()
{
}

bool ButtonBar::Initialize()
{
	m_BarTexId = "ui/windowAtlas.png";
	m_Bar = new D9SliceSprite(m_BarTexId);
	m_Bar->SetPriorityType(DrawPriorityType::Custom, 1);
	SetDrawableObj(m_Bar);

	m_Buttons = std::vector < std::vector<DTile*>>(10, std::vector<DTile*>(8));
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			DTile* btt = new DTile();
			btt->setLocalPosition(sf::Vector2f(i, j) * 16.0f + sf::Vector2f(10, 16));
			m_Buttons[j][i] = btt;
			btt->SetPriorityType(DrawPriorityType::Custom, 2);
			SetDrawableObj(btt);
		}
	}

	m_ButtonTexIds = std::vector < std::vector<std::string>>(10, std::vector<std::string>(8));
	m_ButtonTexCoords = std::vector < std::vector<sf::IntRect>>(10, std::vector<sf::IntRect>(8));

	m_CurrTile = new DTile();
	m_CurrTile->SetPriorityType(DrawPriorityType::Custom, 100);
	m_CurrTile->setLocalPosition({ 16, 200 });
	SetDrawableObj(m_CurrTile);

	return true;
}

void ButtonBar::Reset()
{
	m_Bar->SetTexture(m_BarTexId);
	m_Bar->SetCornerSize(32);
	m_Bar->SetSize({ 16 * 8 + 20, 16 * 20 });
	m_Bar->SetAtlas(sf::FloatRect(0, 32, 16, -16), sf::FloatRect(8, 32, 16, -16), sf::FloatRect(16, 32, 16, -16),
		sf::FloatRect(0, 8, 16, 16), sf::FloatRect(8, 8, 16, 16), sf::FloatRect(16, 8, 16, 16),
		sf::FloatRect(0, 16, 16, 16), sf::FloatRect(8, 16, 16, 16), sf::FloatRect(16, 16, 16, 16));
}

void ButtonBar::Update(float dt)
{
	sf::Vector2f currMousePos = INPUT_MGR->GetMouseViewPos(m_ViewIndex);
	m_HasFocus = false;
	if (m_Bar->GetGlobalBounds().contains(currMousePos))
	{
		m_HasFocus = true;
	}
	else
	{
		return;
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_PrevMouseDown = currMousePos;
	}

	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		sf::Vector2f start(std::min(currMousePos.x, m_PrevMouseDown.x), std::min(currMousePos.y, m_PrevMouseDown.y));
		sf::FloatRect mouserect(start, { std::fabsf(currMousePos.x - m_PrevMouseDown.x), std::fabsf(currMousePos.y - m_PrevMouseDown.y) });

		for (auto& prevselected : m_SelectingButtons)
		{
			m_Buttons[prevselected.y][prevselected.x]->SetColor(ColorPalette::White);
		}

		m_SelectingButtons.clear();
		m_CurrCoords.clear();
		m_CurrIndices.clear();

		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 8; i++)
			{
				if (m_Buttons[j][i]->GetGlobalBounds().intersects(mouserect))
				{
					m_SelectingButtons.push_back({ i,j });
					m_Buttons[j][i]->SetColor(ColorPalette::Gray);
				}
			}
		}

		if (m_SelectingButtons.size() != 0)
		{
			CellIndex offset(100, 100);
			for (auto& selected : m_SelectingButtons)
			{
				m_CurrCoords.push_back(m_ButtonTexCoords[selected.y][selected.x]);
				offset.x = std::min(offset.x, selected.x);
				offset.y = std::min(offset.y, selected.y);
			}
			for (auto& selected : m_SelectingButtons)
			{
				m_CurrIndices.push_back(selected - offset);
			}
			m_CurrTile->SetTextureRect(m_CurrCoords, m_CurrIndices);
		}
	}
}

void ButtonBar::Release()
{
}

void ButtonBar::SetButtonTex(const CellIndex& bttIndex, const std::string& texId)
{
	auto& texres = TEXRESTABLE_MGR->GetTileTexRes(texId);
	m_Buttons[bttIndex.y][bttIndex.x]->SetTexture(texres.filepath);
	m_Buttons[bttIndex.y][bttIndex.x]->SetTextureRect(texres.texcoord);

	m_ButtonTexIds[bttIndex.y][bttIndex.x] = texId;
	m_ButtonTexCoords[bttIndex.y][bttIndex.x] = texres.texcoord;

	m_CurrTile->SetTexture(texres.filepath);
	for (auto& prevselected : m_SelectingButtons)
	{
		m_Buttons[prevselected.y][prevselected.x]->SetColor(ColorPalette::White);
	}
}

const std::list<std::string>& ButtonBar::GetCurrTexIds()
{
	if(m_SelectingButtons.size()== 0)
		return m_CurrTexIds;

	m_CurrTexIds.clear();
	for (auto& currSelected : m_SelectingButtons)
	{
		m_CurrTexIds.push_back(m_ButtonTexIds[currSelected.y][currSelected.x]);
	}
	return m_CurrTexIds;
}
