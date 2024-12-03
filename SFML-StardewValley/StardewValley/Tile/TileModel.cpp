#include "pch.h"
#include "TileModel.h"
#include "TileView.h"

TileModel::TileModel(int layermax, const sf::Vector2u& cellCnt, const sf::Vector2f& cellSize)
	:m_LayerCnt(layermax), m_CellCount(cellCnt), m_CellSize(cellSize)
{
	m_TileInfos.resize(m_LayerCnt);
}

TileModel::~TileModel()
{
}

bool TileModel::Initialize()
{
	for (int layer = 0; layer < m_LayerCnt; layer++)
	{
		InitializeTileInfos(layer);
	}

	return true;
}

void TileModel::Reset()
{
}

void TileModel::Update(float dt)
{

}

void TileModel::SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj)
{
	TileLayer tilelayer = Tile::TileObjLayerToTileLayer(layer);
	m_TileInfos[(int)tilelayer][tileIndex.y][tileIndex.x].owner = tileObj;


	RequestUpdateTile((int)tilelayer, tileIndex);
}

void TileModel::SetTiles(int layer, const std::list<CellIndex>& tiles, const TEXID& id)
{
	for (auto& currIndex : tiles)
	{
		SetTile(layer, currIndex, id);
	}
}

void TileModel::SetTile(int layer, const CellIndex& tileIndex, const TEXID& id, bool isTrueTile)
{
	m_TileInfos[(int)layer][tileIndex.y][tileIndex.x].id = id;
	RequestUpdateTile(layer, tileIndex);
}

bool TileModel::IsValidTileIndex(const CellIndex& tileIndex) const
{
	return tileIndex.x >= 0 && tileIndex.x < (int)m_CellCount.x && tileIndex.y >= 0 && tileIndex.y < (int)m_CellCount.y;
}

const TileInfo& TileModel::GetTileInfo(int layer, const CellIndex& tileIndex) const
{
	return m_TileInfos[layer][tileIndex.y][tileIndex.x];
}

const TileInfo& TileModel::GetTileInfo(const TileLayer& layer, const CellIndex& tileIndex) const
{
	return m_TileInfos[(int)layer][tileIndex.y][tileIndex.x];
}

const TileInfo& TileModel::GetTileInfo(const TileObjLayer& layer, const CellIndex& tileIndex) const
{
	return GetTileInfo(Tile::TileObjLayerToTileLayer(layer), tileIndex);
}

void TileModel::InitializeTileInfos(int layer)
{
	m_TileInfos[layer] = std::vector<std::vector<TileInfo>>(m_CellCount.y, std::vector<TileInfo>(m_CellCount.x));
	for (int j = 0; j < m_CellCount.y; j++)
	{
		for (int i = 0; i < m_CellCount.x; i++)
		{
			auto& currTileInfo = m_TileInfos[layer][j][i];
			currTileInfo.index = { i,j };
			currTileInfo.id = "";
		}
	}
}

//bool TileModel::IsPossibleToPass(const CellIndex& tileIndex)
//{
//	bool passable = true;
//	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
//	{
//		passable &= TileType::Block != m_TileCollInfos[layer][tileIndex.y][tileIndex.x].colliderType;
//	}
//	return passable;
//}

bool TileModel::IsPossibleToSetTile(const CellIndex& tileIndex, int layer, const TEXID& id)
{
	return IsValidTileIndex(tileIndex);
}

void TileModel::RequestUpdateTile(int layer, const CellIndex& tileIndex)
{
#ifdef DEBUG
	if (m_WhenNeedsToUpdateTileFunc)
#endif // DEBUG
		m_WhenNeedsToUpdateTileFunc(layer, tileIndex);
#ifdef DEBUG
	else
		std::cout << "TileModel::RequestUpdateTile-Fail" << std::endl;
#endif // DEBUG
}

//void TileModel::RequestUpdateTileObject(const TileLayer& layer, const CellIndex& tileIndex)
//{
//#ifdef DEBUG
//	if (m_WhenNeedsToUpdateTileObjFunc)
//#endif // DEBUG
//		m_WhenNeedsToUpdateTileObjFunc(layer, tileIndex);
//#ifdef DEBUG
//	else
//		std::cout << "TileModel::RequestUpdateTileObject-Fail" << std::endl;
//#endif // DEBUG
//}
