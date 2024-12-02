#include "pch.h"
#include "TileModel.h"
#include "TileView.h"

TileModel::TileModel(const sf::Vector2u& cellCnt, const sf::Vector2f& cellSize)
	:m_CellCount(cellCnt), m_CellSize(cellSize)
{
	m_TileViewInfos.resize((int)TileViewLayer::Max);
	m_TileCollInfos.resize((int)TileCollLayer::Max);
}

TileModel::~TileModel()
{
}

bool TileModel::Initialize()
{
	for (int layer = 0; layer < (int)TileViewLayer::Max; layer++)
	{
		InitializeTileViewInfoLayer((TileViewLayer)layer);
	}

	for (int layer = 0; layer < (int)TileCollLayer::Max; layer++)
	{
		InitializeTileCollInfoLayer((TileCollLayer)layer);
	}

	return true;
}

void TileModel::Reset()
{
}

void TileModel::Update(float dt)
{

}

const TileViewInfo& TileModel::GetTileViewInfo(const TileViewLayer& layer, const CellIndex& tileIndex) const
{
	return m_TileViewInfos[(int)layer][tileIndex.y][tileIndex.x];
}

const TileCollInfo& TileModel::GetTileCollInfo(const TileCollLayer& layer, const CellIndex& tileIndex) const
{
	return m_TileCollInfos[(int)layer][tileIndex.y][tileIndex.x];
}

void TileModel::SetTiles(const std::list<CellIndex>& tiles, const TileViewLayer& layer, const TEXID& id)
{
	for (auto& currIndex : tiles)
	{
		SetTile(currIndex, layer, id);
	}
}

void TileModel::SetTile(const CellIndex& tileIndex, const TileViewLayer& layer, const TEXID& id, bool isTrueTile)
{
	m_TileViewInfos[(int)layer][tileIndex.y][tileIndex.x].id = id;
	RequestUpdateTile(layer, tileIndex);
}

void TileModel::SetCollisions(const std::list<CellIndex>& tiles, const TileCollLayer& layer, ColliderType type)
{
	for (auto& currIndex : tiles)
	{
		SetCollision(currIndex, layer, type);
	}
}

void TileModel::SetCollision(const CellIndex& tileIndex, const TileCollLayer& layer, ColliderType type)
{
	m_TileCollInfos[(int)layer][tileIndex.y][tileIndex.x].colliderType = type;
}

void TileModel::CollisionTypeMode(const TileCollLayer& layer, ColliderType type)
{
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileCollInfos[(int)layer][j][i];

			if (type == ColliderType::Block && currTileInfo.colliderType == ColliderType::Block)
				RequestColorizeTile(sf::Color(255, 100, 100), { i,j });
			else if(type != ColliderType::Block && currTileInfo.colliderType != ColliderType::Block)
				RequestColorizeTile(sf::Color(100, 100, 255), { i,j });
		}
	}
}


bool TileModel::IsValidTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x > 0 && tileIndex.x < (int)m_CellCount.x - 1 && tileIndex.y > 0 && tileIndex.y < (int)m_CellCount.y - 1;
}

void TileModel::InitializeTileViewInfoLayer(const TileViewLayer& layer)
{
	m_TileViewInfos[(int)layer] = std::vector<std::vector<TileViewInfo>>(m_CellCount.y, std::vector<TileViewInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileViewInfos[(int)layer][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.ower = currTileInfo.index;
			currTileInfo.id = "";
		}
	}
}

void TileModel::InitializeTileCollInfoLayer(const TileCollLayer& layer)
{
	m_TileCollInfos[(int)layer] = std::vector<std::vector<TileCollInfo>>(m_CellCount.y, std::vector<TileCollInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileCollInfos[(int)layer][j][i];
			currTileInfo.index = { i,j };
			//currTileInfo.owner = currTileInfo.index;
			currTileInfo.colliderType = ColliderType::None;
		}
	}
}

bool TileModel::IsPossibleToPass(const CellIndex& tileIndex)
{
	bool passable = true;
	for (int layer = 0; layer < (int)TileCollLayer::Max; layer++)
	{
		passable &= ColliderType::Block != m_TileCollInfos[layer][tileIndex.y][tileIndex.x].colliderType;
	}
	return passable;
}

bool TileModel::IsPossibleToSetTile(const CellIndex& tileIndex, const TileViewLayer& layer, const TEXID& id)
{
	return IsValidTileIndex(tileIndex);
}

void TileModel::RequestUpdateTile(const TileViewLayer& layer, const CellIndex& tileIndex)
{
	if (m_WhenNeedsToUpdateTileFunc)
		m_WhenNeedsToUpdateTileFunc(layer, tileIndex);
}

void TileModel::RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
	if (m_WhenNeedsToColorizeTileFunc)
		m_WhenNeedsToColorizeTileFunc(color, tileIndex);
}
