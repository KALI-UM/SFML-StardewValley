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
	for (int layer = 0; layer < (int)TileViewLayer::Max; layer++)
	{
		LoadTileViewLayerFile((TileViewLayer)layer);
	}

	LoadTileCollisionLayerFile();
	return true;
}

void TileMapSystem::Reset()
{
	m_CurrLayer = TileViewLayer::Terrain;
}

void TileMapSystem::Update(float dt)
{
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const LOT& lot, const std::list<TEXID>& ids)
{
	std::vector<std::vector<TEXID>> id_lot = std::vector<std::vector<TEXID>>(lot.y, std::vector<TEXID>(lot.x));
	auto idit = ids.begin();
	for (int loty = 0; loty < (int)lot.y; loty++)
	{
		for (int lotx = 0; lotx < (int)lot.x; lotx++)
		{
			id_lot[loty][lotx] = *idit;
			idit++;
		}
	}

	for (auto& currIndex : tiles)
	{
		for (int loty = 0; loty < (int)lot.y; loty++)
		{
			for (int lotx = 0; lotx < (int)lot.x; lotx++)
			{
				sf::Vector2i lotoffset(lotx, loty);

				if (mcv_Model->IsValidTileIndex(currIndex + lotoffset))
					mcv_Model->SetTile(currIndex + lotoffset, m_CurrLayer, id_lot[loty][lotx]);
			}
		}
	}
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id)
{
	mcv_Model->SetTiles(tiles, m_CurrLayer, id);
}

void TileMapSystem::BuildTilesById(const CellIndex& tile, const TEXID& id)
{
	mcv_Model->SetTile(tile, m_CurrLayer, id);
}

void TileMapSystem::LoadTileViewLayerFile(TileViewLayer layer)
{
	m_CurrLayer = layer;
	rapidcsv::Document doc("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv", rapidcsv::LabelParams(-1, -1));

	int cellxcnt = std::min((unsigned int)doc.GetColumnCount(), mcv_Model->m_CellCount.x);
	int cellycnt = std::min((unsigned int)doc.GetRowCount(), mcv_Model->m_CellCount.y);

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string strings = doc.GetCell<std::string>(i, j);
			BuildTilesById({ i,j }, strings);
		}
	}
}

void TileMapSystem::SaveTileViewLayerFile(TileViewLayer layer)
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileViewInfo& currInfo = mcv_Model->GetTileViewInfo(layer, { i,j });
			std::string strings = currInfo.id;
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv");
}

void TileMapSystem::LoadTileCollisionLayerFile()
{
	rapidcsv::Document doc("datatables/tileCollInfo_layer" + std::to_string((int)0) + ".csv", rapidcsv::LabelParams(-1, -1));

	int cellxcnt = std::min((unsigned int)doc.GetColumnCount(), mcv_Model->m_CellCount.x);
	int cellycnt = std::min((unsigned int)doc.GetRowCount(), mcv_Model->m_CellCount.y);

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string strings = doc.GetCell<std::string>(i, j);
			mcv_Model->SetCollision({ i,j }, TileCollLayer::Back, Tile::StringToCollisionType(strings));
		}
	}
}

void TileMapSystem::SaveTileCollisionLayerFile()
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileCollInfo& currInfo = mcv_Model->GetTileCollInfo(TileCollLayer::Back, { i,j });
			std::string strings = Tile::CollisionTypeToString(currInfo.collision);
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileCollInfo_layer" + std::to_string((int)0) + ".csv");
}

