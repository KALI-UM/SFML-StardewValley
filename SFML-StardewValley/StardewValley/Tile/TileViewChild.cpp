#include "pch.h"
#include "TileViewChild.h"
#include "TileView.h"
#include "TileModel.h"
#include "TexCoordTable.h"
#include "DTile.h"

TileViewChild::TileViewChild(TileView* view, TileViewType type)
	:mcv_View(view), m_TileViewType(type)
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

	switch (m_TileViewType)
	{
	case TileViewType::Raw:
		InitializeRaw();
		break;
	case TileViewType::TexId:
		InitializeTexId();
		break;
	case TileViewType::Object:
		InitializeTileObject();
		break;
	}

	return true;
}

void TileViewChild::Reset()
{
	switch (m_TileViewType)
	{
	case TileViewType::Raw:
		ResetRaw();
		break;
	case TileViewType::TexId:
		ResetTexId();
		break;
	case TileViewType::Object:
		ResetTileObject();
		break;
	}
}

void TileViewChild::LateUpdate(float dt)
{
}

void TileViewChild::PreRender()
{
	if (m_IsSelfPriorityView && m_NeedPriorityUpdate)
	{
		//타일은 매 프레임마다 순위경쟁을 하지 않고 벡터에서 순서가 정해진다.
		std::sort(m_Drawables.begin(), m_Drawables.end(),
			[&](const DrawableObject* d1, const DrawableObject* d2) { return TileViewChild::SortTile(d1, d2); });
	}
}

void TileViewChild::PostRender()
{
	ResetColorizedTile();
}

void TileViewChild::Release()
{
}

void TileViewChild::ColorizeTile(const sf::Color& color, const CellIndex& tileIndex, bool needReset)
{
	auto tile = m_TileDrawable[tileIndex.y][tileIndex.x];
	if (tile)
	{
		tile->SetFillColor(color);
		if (needReset)
			m_ColorizedTiles.push(tileIndex);
	}
}

void TileViewChild::SetIsSelfPriortyView(bool selfpriority)
{
	m_IsSelfPriorityView = selfpriority;

	if (m_IsSelfPriorityView)
	{
		const auto& cellCount = mcv_View->GetModel()->m_CellCount;
		const auto& cellSize = mcv_View->GetModel()->m_CellSize;
		for (int j = 0; j < (int)cellCount.y; j++)
		{
			for (int i = 0; i < (int)cellCount.x; i++)
			{
				auto tile = m_TileDrawable[j][i];
				if (tile)
					m_TileDrawable[j][i]->SetPriorityType(DrawPriorityType::Custom, j);
			}
		}

		NeedPriorityUpdate();
	}
}

void TileViewChild::InitializeTexId()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;
	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			DTile* tileSprite = new DTile();
			//일단 cellsize가 정사각형이어서 이렇게했다...
			tileSprite->SetUnitSize(cellSize.x);
			tileSprite->setLocalPosition({ (i + 0.5f) * cellSize.x, (j + 1) * cellSize.y });
			SetDrawableObj(tileSprite);
			m_TileDrawable[j][i] = tileSprite;
		}
	}
}

void TileViewChild::InitializeRaw()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			DTile* tileSprite = new DTile();
			//일단 cellsize가 정사각형이어서 이렇게했다...
			tileSprite->SetUnitSize(cellSize.x);
			tileSprite->setLocalPosition({ (i + 0.5f) * cellSize.x, (j + 1) * cellSize.y });
			SetDrawableObj(tileSprite);
			m_TileDrawable[j][i] = tileSprite;
		}
	}
}

void TileViewChild::InitializeTileObject()
{

}

void TileViewChild::ResetTexId()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			auto& tileSprite = m_TileDrawable[j][i];
			const auto& tileInfo = mcv_View->GetModel()->GetTileInfo(m_TileViewIndex, { i,j });
			const auto& texres = TEXRESTABLE_MGR->GetTileTexRes(tileInfo.id);

			tileSprite->SetTexture(texres.filepath);
			tileSprite->SetTextureRect(texres.texcoord);
			tileSprite->SetDebugDraw(false);
			tileSprite->SetTileOrigin({ 0,0 });
		}
	}
}

void TileViewChild::ResetRaw()
{
	const auto& cellCount = mcv_View->GetModel()->m_CellCount;
	const auto& cellSize = mcv_View->GetModel()->m_CellSize;

	for (int j = 0; j < (int)cellCount.y; j++)
	{
		for (int i = 0; i < (int)cellCount.x; i++)
		{
			auto& tileSprite = m_TileDrawable[j][i];
			tileSprite->SetUnitxUnitSize({ 1,1 });
			tileSprite->SetDebugDraw(false);
			tileSprite->SetTileOrigin({ 0,0 });
		}
	}
}

void TileViewChild::ResetTileObject()
{
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

bool TileViewChild::SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const
{
	return dobj1->GetPriorityValue() < dobj2->GetPriorityValue();
}

