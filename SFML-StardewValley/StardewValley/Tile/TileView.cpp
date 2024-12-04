#include "pch.h"
#include "TileView.h"
#include "TexCoordTable.h"
#include "TileModel.h"
#include "TileViewChild.h"
#include "TileGrid.h"
#include "DTile.h"
#include "TileObject.h"

TileView::TileView(TileModel* model)
	:mcv_Model(model)
{
	mcv_Model->SetTileUpdateFunc(std::bind(&TileView::PushToViewUpdateQue, this, std::placeholders::_1, std::placeholders::_2));
	m_TileViewChildren.resize(mcv_Model->m_LayerCnt);
}

TileView::~TileView()
{
}

bool TileView::Initialize()
{
	if (!mcv_Model)return false;
	m_TileGrid->SetCellSize(mcv_Model->m_CellSize);
	m_TileGrid->SetCellCount(mcv_Model->m_CellCount);
	return true;
}

void TileView::Reset()
{
	m_TileGrid->SetTextVisible(false);

}

void TileView::Update(float dt)
{
}

void TileView::LateUpdate(float dt)
{
	UpdateTileSprite();
	UpdateTileObject();
}

void TileView::PostRender()
{
}

void TileView::Release()
{
}

void TileView::SetTileViewIndex(int layer, TileViewChild* child)
{
	GameObject::SetChildObj((GameObject*)child);
	m_TileViewChildren[layer] = child;
	child->m_TileViewIndex = layer;
}

void TileView::SetTileGrid(TileGrid* grid)
{
	GameObject::SetChildObj((GameObject*)grid);
	m_TileGrid = grid;
}

void TileView::SetGridTextSize(float zoom)
{
	zoom = Utils::Clamp(zoom, 10.f, 30.f);
	m_TileGrid->SetTextSize(zoom);
}

void TileView::SetTileViewVisible(int layer, bool visible)
{
	m_TileViewChildren[layer]->SetIsVisible(visible);
}

void TileView::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);
	for (auto& layer : m_TileViewChildren)
	{
		if (layer)
			layer->SetTileTransform(trans);
	}

	m_TileGrid->SetTileTransform(zero, m_TileTransform);
}

sf::Vector2f TileView::GetTileCoordinatedPos(const sf::Vector2f& pos) const
{
	return m_TileTransform.getInverse().transformPoint(pos);
}

CellIndex TileView::GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos) const
{
	sf::Vector2f tilepos = isTilepos ? pos : GetTileCoordinatedPos(pos);
	return sf::Vector2f(tilepos.x / (float)mcv_Model->m_CellSize.x, tilepos.y / (float)mcv_Model->m_CellSize.y).To<int>();
}

//sf::Vector2f TileView::GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex)
//{
//	return 1.5f * sf::Vector2f(mcv_Model->m_CellSize.x * tileIndex.x, mcv_Model->m_CellSize.y * tileIndex.y);
//}
//
//int TileView::GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const
//{
//	return tileIndex.x + tileIndex.y * mcv_Model->m_CellCount.x;
//}


void TileView::ColorizeTile(const sf::Color& color, int layer, const CellIndex& tileIndex)
{
	if (!mcv_Model->IsValidTileIndex(tileIndex))
		return;

	m_TileViewChildren[layer]->ColorizeTile(color, tileIndex);
}

void TileView::ColorizeAllTile(const sf::Color& color, const CellIndex& tileIndex, const UNITxUNIT& uu)
{
	for (int layer = 0; layer < mcv_Model->m_LayerCnt; layer++)
	{
		for (int uuy = 0; uuy < (int)uu.y; uuy++)
		{
			for (int uux = 0; uux < (int)uu.x; uux++)
			{
				ColorizeTile(color, layer, tileIndex + sf::Vector2i(uux, uuy));
			}
		}
	}
}

void TileView::ColorizeTile(const sf::Color& color, int layer, const std::list<CellIndex>& tiles)
{
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		ColorizeTile(color, layer, *it);
	}
}

void TileView::ColorizeAllTiles(const sf::Color& color, const std::list<CellIndex>& tiles)
{
	for (int layer = 0; layer < (int)TileObjLayer::Max; layer++)
	{
		ColorizeTile(color, layer, tiles);
	}
}

void TileView::PushToViewUpdateQue(int layer, const CellIndex& tileIndex)
{
	if (m_TileViewChildren[layer]->m_TileViewType == TileViewType::Raw)
		PushToSpriteUpdateQue(layer, tileIndex);
	else
		PushToTileObjectUpdateQue(layer, tileIndex);
}

void TileView::PushToSpriteUpdateQue(int layer, const CellIndex& tileIndex)
{
	m_SpriteUpdateQueue.push({ layer, tileIndex });
}

void TileView::PushToTileObjectUpdateQue(int layer, const CellIndex& tileIndex)
{
	m_TileObjUpdateQueue.push({ layer, tileIndex });
}

void TileView::UpdateTileSprite()
{
	while (!m_SpriteUpdateQueue.empty())
	{
		int currlayer = m_SpriteUpdateQueue.front().first;
		sf::Vector2i& currIndex = m_SpriteUpdateQueue.front().second;
		auto& currTile = m_TileViewChildren[(int)currlayer]->m_TileDrawable[currIndex.y][currIndex.x];
		auto& currTileInfo = mcv_Model->GetTileInfo(currlayer, currIndex);
		auto& currTexRes = TEXRESTABLE_MGR->GetTileTexRes(currTileInfo.id);

		currTile->SetTexture(currTexRes.filepath);
		currTile->SetTextureRect(currTexRes.texcoord);
		currTile->SetOrigin(OriginType::BC, m_TileOffset);
		m_SpriteUpdateQueue.pop();
	}
}

void TileView::UpdateTileObject()
{
	while (!m_TileObjUpdateQueue.empty())
	{
		int currlayer = m_TileObjUpdateQueue.front().first;
		sf::Vector2i& currIndex = m_TileObjUpdateQueue.front().second;
		auto& currTileInfo = mcv_Model->GetTileInfo(currlayer, currIndex);
		if (currTileInfo.owner)
		{
			m_TileViewChildren[currlayer]->m_TileDrawable[currIndex.y][currIndex.x] = currTileInfo.owner->GetDTile();
			currTileInfo.owner->setPosition({ currIndex.x * mcv_Model->m_CellSize.x, currIndex.y * mcv_Model->m_CellSize.y });
		}
		else
			m_TileViewChildren[currlayer]->m_TileDrawable[currIndex.y][currIndex.x] = nullptr;

		m_TileObjUpdateQueue.pop();
	}
}
