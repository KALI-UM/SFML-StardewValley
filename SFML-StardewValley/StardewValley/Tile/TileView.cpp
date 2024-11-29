#include "pch.h"
#include "TileView.h"
#include "TileModel.h"
#include "TileViewChild.h"
#include "TileGrid.h";
#include "DTile.h"
#include "TexCoordTable.h"

TileView::TileView(TileModel* model)
	:mcv_Model(model)
{
	mcv_Model->SetTileUpdateFunc(std::bind(&TileView::PushToSpriteUpdateQue, this, std::placeholders::_1, std::placeholders::_2));
	//mcv_Model->SetTempEffectTileUpdateFunc(std::bind(&TileView::PushToTempEffectUpdateQue, this, std::placeholders::_1));
	m_LayerViews.resize(mcv_Model->m_MaxLayer);
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
}

void TileView::PostRender()
{
}

void TileView::Release()
{
}

void TileView::SetTileLayerView(const TileLayer& layer, TileViewChild* child)
{
	GameObject::SetChildObj((GameObject*)child);
	m_LayerViews[(int)layer] = child;
	child->m_Layer = layer;
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

void TileView::SetTileLayerVisible(const TileLayer& layer, bool visible)
{
	m_LayerViews[(int)layer]->SetIsVisible(visible);
}

void TileView::SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans)
{
	m_TileTransform = trans;
	setPosition(zero);
	for (auto& layer : m_LayerViews)
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


void TileView::ColorizeTile(const sf::Color& color, const TileLayer& layer, const CellIndex& tileIndex)
{
	if (!mcv_Model->IsValidTileIndex(tileIndex))
		return;

	m_LayerViews[(int)layer]->ColorizeTile(color, tileIndex);
}

void TileView::ColorizeTile(const sf::Color& color, const TileLayer& layer, const std::list<CellIndex>& tiles)
{
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		ColorizeTile(color, layer, *it);
	}
}

void TileView::PushToSpriteUpdateQue(const TileLayer& depth, const CellIndex& tileIndex)
{
	m_SpriteUpdateQueue.push({ depth, tileIndex });
}

void TileView::UpdateTileSprite()
{
	while (!m_SpriteUpdateQueue.empty())
	{
		TileLayer& currlayer = m_SpriteUpdateQueue.front().first;
		sf::Vector2i& currIndex = m_SpriteUpdateQueue.front().second;
		auto& currTile = m_LayerViews[(int)currlayer]->m_TileDrawable[currIndex.y][currIndex.x];
		auto& currTileInfo = mcv_Model->GetTileInfo(currlayer, currIndex);
		auto& currTexRes = TILETEXRESTABLE_MGR->GetTileTexRes(currTileInfo.id);

		currTile->SetTexture(currTexRes.filepath);
		currTile->SetTexureRect(currTexRes.texcoord);
		currTile->SetOrigin(OriginType::BL, m_TileOffset);
		m_SpriteUpdateQueue.pop();
	}
}
