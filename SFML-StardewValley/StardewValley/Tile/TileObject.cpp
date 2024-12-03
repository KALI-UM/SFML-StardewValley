#include "pch.h"
#include "TileObject.h"
#include "DTile.h"
#include "TexCoordTable.h"

TileObject::TileObject()
{
}

TileObject::~TileObject()
{
}

bool TileObject::Initialize()
{
	m_TileSprite = new DTile();
	SetDrawableObj(m_TileSprite, false);

	return true;
}

void TileObject::LoadTileObject()
{
	
	std::list<sf::IntRect> tileTexrects;
	std::list<CellIndex> tileIndices;

	for (int j = 0; j < m_Data->tileTypeMap.size(); j++)
	{
		for (int i = 0; i < m_Data->tileTypeMap[j].size(); i++)
		{
			const TileObjectData::UnitData& currUnit = m_Data->tileTypeMap[j][i];
			m_TileTypes.push_back({ currUnit.offset, currUnit.type });
			
			if (currUnit.offset == CellIndex(0, 0))
			{
				m_TLOffset = CellIndex(-i, -j);
			}

			const auto& currTexRes = TEXRESTABLE_MGR->GetTileTexRes(currUnit.texid);
			tileTexrects.push_back(currTexRes.texcoord);
			tileIndices.push_back({ i,j });
		}
	}

	m_TileSprite->SetTextureRect(tileTexrects, tileIndices);
}

const TileType& TileObject::GetTileType(const CellIndex& tileIndex) const
{
	CellIndex localIndex = tileIndex - (m_TileIndex + m_TLOffset);
	return m_Data->tileTypeMap[localIndex.y][localIndex.x].type;
}


