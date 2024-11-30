#include "pch.h"
#include "TileGrid.h"
#include "DSprite.h"
#include "DLine.h"
#include "DText.h"

TileGrid::TileGrid()
{
}

TileGrid::~TileGrid()
{
}

bool TileGrid::Initialize()
{
	std::vector<sf::Vector2f> rowPoints;
	std::vector<sf::Vector2f> colPoints;
	for (int i = 0; i <= (int)m_CellCount.y; i++)
	{
		rowPoints.push_back({ 0,					i * m_CellSize.y });
		rowPoints.push_back({ m_CellSize.x * m_CellCount.x,	i * m_CellSize.y });

	}
	for (int i = 0; i <= (int)m_CellCount.x; i++)
	{
		colPoints.push_back({ i * m_CellSize.x,		0 });
		colPoints.push_back({ i * m_CellSize.x ,	m_CellSize.y * m_CellCount.y });
	}
	m_RowLine = new DLine(rowPoints.data(), rowPoints.size(), sf::Color::Red, false);
	m_ColLine = new DLine(colPoints.data(), colPoints.size(), sf::Color::Blue, false);
	SetDrawableObj(m_RowLine);
	SetDrawableObj(m_ColLine);

	for (int j = 0; j < (int)m_CellCount.y; j++)
	{
		for (int i = 0; i < (int)m_CellCount.x; i++)
		{
			DText* text = new DText("core/DOSGothic.ttf", "(" + std::to_string(i) + "," + std::to_string(j) + ")", 10);
			text->SetDebugDraw(false);
			text->SetColor(ColorPalette::Black);
			text->setLocalPosition({ i * m_CellSize.x, j * m_CellSize.y });
			m_Texts.push_back(text);
			SetDrawableObj(text);
		}
	}
	return false;
}

void TileGrid::Reset()
{

}

void TileGrid::Update(float dt)
{
}

void TileGrid::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);

	for (int index = 0; index < m_RowLine->GetPointsSize(); index++)
	{
		m_RowLine->SetPoint(index, m_TileTransform.transformPoint(m_RowLine->GetPoint(index)));
	}

	for (int index = 0; index < m_ColLine->GetPointsSize(); index++)
	{
		m_ColLine->SetPoint(index, m_TileTransform.transformPoint(m_ColLine->GetPoint(index)));
	}

	for (int i = 0; i < m_Texts.size(); i++)
	{
		m_Texts[i]->setLocalPosition(m_TileTransform.transformPoint(m_Texts[i]->getLocalPosition()));
	}
}

void TileGrid::SetTextVisible(bool visible)
{
	for (int i = 0; i < m_Texts.size(); i++)
	{
		m_Texts[i]->SetIsVisible(visible);
	}
}

void TileGrid::SetTextSize(float size)
{
	for (int i = 0; i < m_Texts.size(); i++)
	{
		m_Texts[i]->SetCharacterSize(size);
		m_Texts[i]->SetOrigin(OriginType::TL);
	}
}
