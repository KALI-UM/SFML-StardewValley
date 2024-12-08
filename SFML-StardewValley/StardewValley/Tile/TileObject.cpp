#include "pch.h"
#include "TileObject.h"
#include "DTile.h"
#include "TileObjDataTable.h"
#include "TexCoordTable.h"
#include "ItemGenerator.h"
#include "TileObjectSystem.h"


TileObject::TileObject()
{
}

TileObject::~TileObject()
{
}

bool TileObject::IsNeedToReturn()
{
	return false;
}

void TileObject::InitForObjectPool()
{
	this->INITIALIZE();
}

void TileObject::ResetForObjectPool()
{
	this->RESET();
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
	m_Data = nullptr;
	m_TileTypes.clear();
	//LoadTileObject();
}

void TileObject::LoadTileObject(const TOBJID& id)
{
	m_TObjId = id;
	m_Data = &TOBJDATATABLE_MGR->GetTileObjectData(m_TObjId);
	m_HitCount = m_Data->need;
	if (m_HitCount == 0)m_HitCount = -1;

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

bool TileObject::IsToolInteractive() const
{
	return (m_Data->objType == TileObjLayer::Paths);
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

void TileObject::ToolInteraction()
{
	m_HitCount--;
	if (m_HitCount == 0)
	{
		ITEMGENERATOR->DropItemToMap(m_Data->itemid, sf::Vector2f(m_TileIndex.x * 16.0f + 8.0f, m_TileIndex.y * 16.0f + 8.0f));
		m_TileSystem->RemoveTileObject(m_Data->objType, m_TileIndex, this);
	}
}




