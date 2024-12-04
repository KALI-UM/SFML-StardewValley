#include "pch.h"
#include "TileObjectSystem.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileObject.h"

TileObjectSystem::TileObjectSystem(TileModel* model, TileView* view)
	:mcv_Model(model), mcv_View(view)
{
}

TileObjectSystem::~TileObjectSystem()
{
}

bool TileObjectSystem::Initialize()
{
	m_TileObjects.resize((int)TileObjLayer::Max);
	LoadTileLayerRawFile();


	SetTileColorizeFunc(std::bind(&TileView::ColorizeAllTile, mcv_View, std::placeholders::_1, std::placeholders::_2, sf::Vector2u(1, 1)));

	return true;
}

void TileObjectSystem::Reset()
{
}

void TileObjectSystem::Update(float dt)
{
	if (INPUT_MGR->GetKey(sf::Keyboard::Num1))
	{
		ColorizePassableTile();
	}
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
			mcv_Model->SetTile((int)TileLayer::Terrain, { i,j }, texid);
		}
	}
}

void TileObjectSystem::LoadTileLayerObjectFile()
{
}

CellIndex TileObjectSystem::GetTileCoordinatedTileIndex(const sf::Vector2f& pos) const
{
	return mcv_View->GetTileCoordinatedIndex(pos);
}

TileObject* TileObjectSystem::GetTileObjectByTileIndex(TileLayer layer, const CellIndex& tileIndex)const
{
	return mcv_Model->GetTileInfo(layer, tileIndex).owner;
}

TileType TileObjectSystem::GetTileTypeByTileIndex(TileLayer layer, const CellIndex& tileIndex)const
{
	TileObject* tobj = GetTileObjectByTileIndex(layer, tileIndex);
	if (tobj)
	{
		return tobj->GetTileTypeByTileIndex(tileIndex);
	}
	else
	{
		return TileType::None;
	}
}

bool TileObjectSystem::IsPossibleToSetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex)
{
	bool result = true;
	result &= mcv_Model->IsValidTileIndex(tileIndex);
	result &= mcv_Model->GetTileInfo(layer, tileIndex).owner == nullptr;
	return result;
}

bool TileObjectSystem::IsPossibleToPass(const CellIndex& tileIndex)
{
	if (!mcv_Model->IsValidTileIndex(tileIndex))
		return false;

	bool result = true;
	for (int layer = 0; layer < mcv_Model->m_LayerCnt; layer++)
	{
		const TileObject* tobj = mcv_Model->GetTileInfo(layer, tileIndex).owner;
		if (tobj)
		{
			result &= tobj->IsPassableTileByTileIndex(tileIndex);
		}
	}
	return result;
}

void TileObjectSystem::ColorizePassableTile()
{
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (IsPossibleToPass({ i,j }))
			{
				RequestColorizeTile(ColorPalette::Blue, { i,j });
			}
		}
	}
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

void TileObjectSystem::RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
#ifdef DEBUG
	if (m_WhenNeedsToColorizeTileFunc)
#endif // DEBUG
		m_WhenNeedsToColorizeTileFunc(color, tileIndex);
#ifdef DEBUG
	else
		std::cout << "TileModel::RequestColorizeTile-Fail" << std::endl;
#endif // DEBUG
}
