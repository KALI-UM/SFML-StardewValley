#include "pch.h"
#include "TileObject.h"
#include "DTile.h"
#include "TileObjDataTable.h"
#include "TexCoordTable.h"


TileObject::TileObject(const TOBJID& id)
	:m_TOBjId(id)
{
}

TileObject::~TileObject()
{
}

bool TileObject::Initialize()
{
	m_TileSprite = new DTile();
	SetDrawableObj(m_TileSprite);


	return true;
}

void TileObject::Reset()
{
	SetIsVisible(false);
	LoadTileObject();
}

void TileObject::LoadTileObject()
{
	m_Data = &TOBJDATATABLE_MGR->GetTileObjectData(m_TOBjId);

	std::list<sf::IntRect> tileTexrects;
	std::list<CellIndex> tileIndices;

	for (int j = 0; j < m_Data->tileTypeMap.size(); j++)
	{
		for (int i = 0; i < m_Data->tileTypeMap[j].size(); i++)
		{
			const TileObjData::UnitData& currUnit = m_Data->tileTypeMap[j][i];
			m_TileTypes.push_back({ -m_Data->originIndex + sf::Vector2i(i,j), currUnit.type });

			m_TileSprite->SetTexture(currUnit.texres->filepath);
			tileTexrects.push_back(currUnit.texres->texcoord);
			tileIndices.push_back({ i,j });
		}
	}
	m_TileSprite->SetTextureRect(tileTexrects, tileIndices);
	m_TileSprite->SetTileOrigin(m_Data->originIndex);
	m_TileSprite->SetPriorityType(DrawPriorityType::Y);
}

const TileType& TileObject::GetTileTypeByTileIndex(const CellIndex& tileIndex) const
{
	CellIndex localIndex = tileIndex - (m_TileIndex - m_Data->originIndex);
	return m_Data->tileTypeMap[localIndex.y][localIndex.x].type;
}

const std::string& TileObject::GetTileSubtypeByTileIndex(const CellIndex& tileIndex) const
{
	CellIndex localIndex = tileIndex - (m_TileIndex - m_Data->originIndex);
	return m_Data->tileTypeMap[localIndex.y][localIndex.x].subtype;
}

bool TileObject::IsPassableTileByTileIndex(const CellIndex& tileIndex) const
{
	const TileType& type = GetTileTypeByTileIndex(tileIndex);
	return (type != TileType::ImpassableInteractive && type != TileType::Wall && type != TileType::Water);
}

bool TileObject::IsInteractiveTileByTileIndex(const CellIndex& tileIndex) const
{
	const TileType& type = GetTileTypeByTileIndex(tileIndex);
	return (type == TileType::ImpassableInteractive || type == TileType::PassableInteractive);
}




