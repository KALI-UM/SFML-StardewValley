#include "pch.h"
#include "TileObjectSystem.h"
#include "TileModel.h"

TileObjectSystem::TileObjectSystem(TileModel* model)
	:mcv_Model(model)
{
}

TileObjectSystem::~TileObjectSystem()
{
}

bool TileObjectSystem::Initialize()
{
	m_TileObjects.resize((int)TileObjLayer::Max);

	LoadTileLayerRawFile();

	return true;
}

void TileObjectSystem::Reset()
{
}

void TileObjectSystem::Update(float dt)
{
}

void TileObjectSystem::LoadTileLayerRawFile()
{
	rapidcsv::Document terrdoc("datatables/tileInfo_layer0.csv", rapidcsv::LabelParams(-1, -1));
	int cellxcnt = std::min((unsigned int)terrdoc.GetColumnCount(), mcv_Model->m_CellCount.x);
	int cellycnt = std::min((unsigned int)terrdoc.GetRowCount(), mcv_Model->m_CellCount.y);

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string texid = terrdoc.GetCell<std::string>(i, j);
			mcv_Model->SetTile((int)TileLayer::Terrain, {i,j}, texid);
		}
	}

	rapidcsv::Document backdoc("datatables/tileInfo_layer1.csv", rapidcsv::LabelParams(-1, -1));
	cellxcnt = std::min((unsigned int)backdoc.GetColumnCount(), mcv_Model->m_CellCount.x);
	cellycnt = std::min((unsigned int)backdoc.GetRowCount(), mcv_Model->m_CellCount.y);

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string texid = backdoc.GetCell<std::string>(i, j);
			mcv_Model->SetTile((int)TileLayer::Back, { i,j }, texid);
		}
	}
}

void TileObjectSystem::LoadTileLayerObjectFile()
{
}

bool TileObjectSystem::IsPossibleToSetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex)
{
	bool result = true;
	result &= mcv_Model->IsValidTileIndex(tileIndex);
	result &= mcv_Model->GetTileInfo(layer, tileIndex).owner == nullptr;
	return result;
}

void TileObjectSystem::IsPossibleToPass()
{
}

void TileObjectSystem::SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj)
{
	m_TileObjects[(int)layer].push_back(tileObj);
	mcv_Model->SetTileObject(layer, tileIndex, tileObj);
}

void TileObjectSystem::RemoveTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj)
{
	m_TileObjects[(int)layer].remove(tileObj);
	mcv_Model->SetTileObject(layer, tileIndex, nullptr);
}
