#include "pch.h"
#include "TileViewChild.h"
#include "TileView.h"
#include "TileModel.h"
#include "TexCoordTable.h"
#include "DTile.h"

TileViewChild::TileViewChild(TileView* view)
	:mcv_View(view)
{
}

TileViewChild::~TileViewChild()
{
}

bool TileViewChild::Initialize()
{
	if (!mcv_View)return false;

	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	m_TileDrawable = std::vector<std::vector<DTile*>>(cellCount.y, std::vector<DTile*>(cellCount.x));
	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			DTile* tileSprite = new DTile();
			tileSprite->SetOrigin(OriginType::BC, mcv_View->m_TileOffset);
			tileSprite->setLocalPosition({ (i)*cellSize.x, (j + 1) * cellSize.y });
			SetDrawableObj(tileSprite);
			m_TileDrawable[j][i] = tileSprite;
		}
	}
	
	return true;
}

void TileViewChild::Reset()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			auto& tileSprite = m_TileDrawable[j][i];
			const auto& tileInfo = mcv_View->GetModel()->GetTileInfo(m_Layer, { i,j });
			const auto& texres = TEXRESTABLE_MGR->GetTileTexRes(tileInfo.id);

			tileSprite->SetTexture(texres.filepath);
			tileSprite->SetTexureRect(texres.texcoord);
			tileSprite->SetDebugDraw(false);
			tileSprite->SetOrigin(OriginType::BC, mcv_View->m_TileOffset);
		}
	}
}

void TileViewChild::LateUpdate(float dt)
{
}

void TileViewChild::PreRender()
{
	//if (m_NeedPriorityUpdate)
	//{
	//	//타일은 매 프레임마다 순위경쟁을 하지 않고 벡터에서 순서가 정해진다.
	//	std::sort(m_Drawables.begin(), m_Drawables.end(),
	//		[&](const DrawableObject* d1, const DrawableObject* d2) { return TileViewChild::SortTile(d1, d2); });
	//}
}

void TileViewChild::PostRender()
{
	ResetColorizedTile();
}

void TileViewChild::Release()
{
}

void TileViewChild::ColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
	auto tile =  m_TileDrawable[tileIndex.y][tileIndex.x];
	tile->SetFillColor(color);
	m_ColorizedTiles.push(tileIndex);
}

void TileViewChild::ResetColorizedTile()
{
	while (!m_ColorizedTiles.empty())
	{
		auto tile = m_TileDrawable[m_ColorizedTiles.front().y][m_ColorizedTiles.front().x];
		tile->SetFillColor(ColorPalette::White);
		m_ColorizedTiles.pop();
	}
}

void TileViewChild::SetTileTransform(const sf::Transform& trans)
{
	for (auto& tile : m_Drawables)
	{
		tile->setLocalPosition(trans.transformPoint(tile->getLocalPosition()));
	}
}

//bool TileViewChild::SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const
//{
//	return dobj1->GetPriorityValue() < dobj2->GetPriorityValue();
//}

