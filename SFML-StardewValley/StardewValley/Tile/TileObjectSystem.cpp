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
	SetTileColorizeFunc(std::bind(&TileView::ColorizeTile, mcv_View, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	m_TileObjects.resize((int)TileObjLayer::Max);
	//LoadTileLayerRawFile();


	return true;
}

void TileObjectSystem::Reset()
{
	SetLightLayerColor(sf::Color(0, 0, 0, 0));
	mcv_View->SetTileViewSelfPriority((int)TileLayer::Terrain, true);
	mcv_View->SetTileViewSelfPriority((int)TileLayer::WaterEffect, true);
}

void TileObjectSystem::Update(float dt)
{
}

void TileObjectSystem::PostRender()
{
	mcv_View->SetTileViewVisible((int)TileLayer::Debug, false);
}

void TileObjectSystem::LoadTileLayerRawFile()
{
	rapidcsv::Document terrdoc("datatables/TileObj/temp/Terraintex.csv", rapidcsv::LabelParams(-1, -1));
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

void TileObjectSystem::LoadTileLayerRawFile(const std::string& terrainfile)
{
	rapidcsv::Document terrdoc(terrainfile, rapidcsv::LabelParams(-1, -1));
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

bool TileObjectSystem::IsPossibleToPass(const CellIndex& tileIndex) const
{
	if (!mcv_Model->IsValidTileIndex(tileIndex))
		return false;

	for (int layer = 0; layer < mcv_Model->m_LayerCnt; layer++)
	{
		const TileObject* tobj = mcv_Model->GetTileInfo(layer, tileIndex).owner;
		if (tobj)
		{
			if (!tobj->IsPassableTileByTileIndex(tileIndex))
				return false;
		}
	}
	return true;
}

bool TileObjectSystem::IsInteractive(const CellIndex& tileIndex) const
{
	if (!mcv_Model->IsValidTileIndex(tileIndex))
		return false;

	for (int layer = 0; layer < mcv_Model->m_LayerCnt; layer++)
	{
		const TileObject* tobj = mcv_Model->GetTileInfo(layer, tileIndex).owner;
		if (tobj)
		{
			if (tobj->IsInteractiveTileByTileIndex(tileIndex))
				return true;
		}
	}
	return false;
}

void TileObjectSystem::ColorizePassableTile()
{
	mcv_View->SetTileViewVisible((int)TileLayer::Debug, true);
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (IsPossibleToPass({ i,j }))
			{
				RequestColorizeTile(sf::Color(0, 0, 255, 100), (int)TileLayer::Debug, { i,j }, true);
			}
			else
			{
				RequestColorizeTile(sf::Color(255, 0, 0, 100), (int)TileLayer::Debug, { i,j }, true);
			}
		}
	}
}

void TileObjectSystem::ColorizeInteractiveTile()
{
	mcv_View->SetTileViewVisible((int)TileLayer::Debug, true);
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (IsInteractive({ i,j }))
			{
				RequestColorizeTile(sf::Color(0, 255, 0, 100), (int)TileLayer::Debug, { i,j }, true);
			}
			else
			{
				RequestColorizeTile(sf::Color(0, 0, 0, 0), (int)TileLayer::Debug, { i,j }, true);
			}
		}
	}
}

void TileObjectSystem::SetLightLayerColor(const sf::Color& color)
{
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			RequestColorizeTile(color, (int)TileLayer::Light, { i,j }, false);
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

void TileObjectSystem::RequestColorizeTile(const sf::Color& color, int layer, const CellIndex& tileIndex, bool needReset)
{
#ifdef DEBUG
	if (m_WhenNeedsToColorizeTileFunc)
#endif // DEBUG
		m_WhenNeedsToColorizeTileFunc(color, layer, tileIndex, needReset);
#ifdef DEBUG
	else
		std::cout << "TileModel::RequestColorizeTile-Fail" << std::endl;
#endif // DEBUG
}
