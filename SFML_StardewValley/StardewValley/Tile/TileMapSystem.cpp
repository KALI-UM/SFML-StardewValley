#include "pch.h"
#include "TileMapSystem.h"
#include "TileModel.h"

TileMapSystem::TileMapSystem(TileModel* model)
	:mcv_Model(model)
{
	//Tile::Initialize();
}

TileMapSystem::~TileMapSystem()
{
}

bool TileMapSystem::Initialize()
{
	for (int layer = 0; layer < (int)TileLayer::Max; layer++)
	{
		LoadTileLayerFile((TileLayer)layer);
	}
	return true;
}

void TileMapSystem::Reset()
{
}

void TileMapSystem::Update(float dt)
{
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id)
{
	mcv_Model->SetTiles(tiles, m_CurrLayer,id);
}

void TileMapSystem::BuildTilesById(const CellIndex& tile, const TEXID& id)
{
	mcv_Model->SetTile(tile, m_CurrLayer, id);
}

void TileMapSystem::LoadTileLayerFile(TileLayer layer)
{
}

void TileMapSystem::SaveTileLayerFile(TileLayer layer)
{
}

void TileMapSystem::InitializeActionSet()
{

}

