#include "pch.h"
#include "TileObjectSystem.h"
#include "TileModel.h"
#include "TileView.h"
#include "TileObject.h"
#include "ObjectPool.h"
#include "Scene_InGame.h"


TileObjectSystem::TileObjectSystem(TileModel* model, TileView* view, Scene_InGame* scene)
	:mcv_Model(model), mcv_View(view), m_CurrInGameScene(scene)
{
}

TileObjectSystem::~TileObjectSystem()
{
}

bool TileObjectSystem::Initialize()
{
	SetTileColorizeFunc(std::bind(&TileView::ColorizeTile, mcv_View, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

	//LoadTileLayerRawFile();
	m_TileObjects.resize((int)TileObjLayer::Max);
	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		m_TileObjects[layer].Initialize(dynamic_cast<SceneBase*>(m_CurrInGameScene), 20, ExpandOption::MakeNew, layer);
	}

	return true;
}

void TileObjectSystem::Reset()
{
	SetEffectLayerColor(sf::Color(0, 0, 0, 0));
	mcv_View->SetTileViewSelfPriority((int)ViewLayer::Terrain, true);
	mcv_View->SetTileViewSelfPriority((int)ViewLayer::WaterEffect, true);
	mcv_View->SetTileViewSelfPriority((int)ViewLayer::Debug, true);
	mcv_View->SetTileViewSelfPriority((int)ViewLayer::Front, true);

	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		m_TileObjects[layer].Reset();
	}
}

void TileObjectSystem::Update(float dt)
{
	if (m_EffectLayerTimer != 0.0f)
	{
		m_EffectLayerTimer += dt;
		SetEffectLayerColor(Utils::Lerp(m_CurrentColor, m_TargetColor, m_EffectLayerTimer / m_EffectLayerEndTime));

		if (m_EffectLayerTimer > m_EffectLayerEndTime)
			m_EffectLayerTimer = 0.0f;
	}

	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		m_TileObjects[layer].Update(dt);
	}
}

void TileObjectSystem::PostRender()
{
	mcv_View->SetTileViewVisible((int)ViewLayer::Debug, false);
}

void TileObjectSystem::Release()
{
	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		m_TileObjects[layer].Exit();
	}
}

//void TileObjectSystem::LoadTileLayerRawFile()
//{
//	rapidcsv::Document terrdoc("datatables/TileObj/temp/Terraintex.csv", rapidcsv::LabelParams(-1, -1));
//	int cellxcnt = std::min((unsigned int)terrdoc.GetColumnCount(), mcv_Model->m_CellCount.x);
//	int cellycnt = std::min((unsigned int)terrdoc.GetRowCount(), mcv_Model->m_CellCount.y);
//
//	for (int j = 0; j < cellycnt; j++)
//	{
//		for (int i = 0; i < cellxcnt; i++)
//		{
//			std::string texid = terrdoc.GetCell<std::string>(i, j);
//			mcv_Model->SetTile((int)ViewLayer::Terrain, { i,j }, texid);
//		}
//	}
//}

void TileObjectSystem::SaveTileObjects(SceneRawFile* savedate)
{
	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		const auto& tobjlist = m_TileObjects[layer].GetUsingObjects();
		for (auto& tobj : tobjlist)
		{
			SceneRawFile::TObjInfo tobjinfo;
			tobjinfo.layer = Tile::TileObjLayerToString((TileObjLayer)layer);
			tobjinfo.index = tobj->GetTileIndex();
			tobjinfo.tobjId = tobj->GetTObjId();
			savedate->tileObjects.push_back({ tobjinfo });
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
			mcv_Model->SetTile((int)ViewLayer::Terrain, { i,j }, texid);
		}
	}
}

CellIndex TileObjectSystem::GetTileCoordinatedTileIndex(const sf::Vector2f& pos) const
{
	return mcv_View->GetTileCoordinatedIndex(pos);
}

TileObject* TileObjectSystem::GetTileObjectByTileIndex(ViewLayer layer, const CellIndex& tileIndex)const
{
	return mcv_Model->GetTileInfo(layer, tileIndex).owner;
}

TileType TileObjectSystem::GetTileTypeByTileIndex(ViewLayer layer, const CellIndex& tileIndex)const
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

const std::string& TileObjectSystem::GetTileSubtypeByTileIndex(const CellIndex& tileIndex) const
{
	for (int layer = (int)ViewLayer::Front; layer >= (int)ViewLayer::Back; layer--)
	{
		TileObject* curr = GetTileObjectByTileIndex((ViewLayer)layer, tileIndex);
		if (curr)
		{
			return curr->GetTileSubtypeByTileIndex(tileIndex);
		}
	}
	return empty;
}

const std::string& TileObjectSystem::GetTileSubtypeByTileIndex(ViewLayer layer, const CellIndex& tileIndex) const
{
	TileObject* tobj = GetTileObjectByTileIndex(layer, tileIndex);
	if (tobj)
	{
		return tobj->GetTileSubtypeByTileIndex(tileIndex);
	}
	else
	{
		return empty;
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
	mcv_View->SetTileViewVisible((int)ViewLayer::Debug, true);
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (IsPossibleToPass({ i,j }))
			{
				RequestColorizeTile(sf::Color(0, 0, 255, 100), (int)ViewLayer::Debug, { i,j }, true);
			}
			else
			{
				RequestColorizeTile(sf::Color(255, 0, 0, 100), (int)ViewLayer::Debug, { i,j }, true);
			}
		}
	}
}

void TileObjectSystem::ColorizeInteractiveTile()
{
	mcv_View->SetTileViewVisible((int)ViewLayer::Debug, true);
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			if (IsInteractive({ i,j }))
			{
				RequestColorizeTile(sf::Color(0, 255, 0, 100), (int)ViewLayer::Debug, { i,j }, true);
			}
			else
			{
				RequestColorizeTile(sf::Color(0, 0, 0, 0), (int)ViewLayer::Debug, { i,j }, true);
			}
		}
	}
}

void TileObjectSystem::SetEffectLayerColor(const sf::Color& curr, const sf::Color& tar, float timer)
{
	m_EffectLayerEndTime = timer;
	m_CurrentColor = curr;
	m_TargetColor = tar;
	m_EffectLayerTimer = 0.01f;
}

void TileObjectSystem::SetEffectLayerColor(const sf::Color& color)
{
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			RequestColorizeTile(color, (int)ViewLayer::Effect, { i,j }, false);
		}
	}
}

void TileObjectSystem::SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj)
{
	tileObj->SetIsVisible(true);
	tileObj->SetTileIndex(tileIndex);
	tileObj->setPosition({ (tileIndex.x + 0.5f) * mcv_Model->m_CellSize.x, (tileIndex.y + 1.0f) * mcv_Model->m_CellSize.y });
	mcv_Model->SetTileObject(layer, tileIndex, tileObj);
}

void TileObjectSystem::SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, const std::string& objId)
{
	TileObject* tobj = m_TileObjects[(int)layer].Take();
	tobj->LoadTileObject(objId);
	tobj->SetTileSystem(this);

	tobj->SetIsVisible(true);
	tobj->SetTileIndex(tileIndex);
	tobj->setPosition({ (tileIndex.x + 0.5f) * mcv_Model->m_CellSize.x, (tileIndex.y + 1.0f) * mcv_Model->m_CellSize.y });
	mcv_Model->SetTileObject(layer, tileIndex, tobj);
}

void TileObjectSystem::RemoveTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj)
{
	tileObj->SetIsVisible(false);
	mcv_Model->RemoveTileObject(layer, tileIndex, tileObj);
	m_TileObjects[(int)layer].Return(tileObj);
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
