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
	m_CurrLayer = TileLayer::Terrain;
}

void TileMapSystem::Update(float dt)
{
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id)
{
	mcv_Model->SetTiles(tiles, m_CurrLayer, id);
}

void TileMapSystem::BuildTilesById(const CellIndex& tile, const TEXID& id)
{
	mcv_Model->SetTile(tile, m_CurrLayer, id);
}

void TileMapSystem::LoadTileLayerFile(TileLayer layer)
{
	m_CurrLayer = layer;
	rapidcsv::Document doc("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			std::string strings = doc.GetCell<std::string>(i,j);
			BuildTilesById({ i,j }, strings);		
		}
	}
}

void TileMapSystem::SaveTileLayerFile(TileLayer layer)
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileInfo& currInfo = mcv_Model->GetTileInfo(layer, { i,j });
			std::string strings = currInfo.id;
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv");
}

void TileMapSystem::InitializeActionSet()
{

}

