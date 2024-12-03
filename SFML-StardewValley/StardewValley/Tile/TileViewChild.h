#pragma once
#include "Tile/Tile.h"

class TileView;
class DTile;
enum class TileViewType
{
	Raw,						//texid로 childview에서 직접 그린다
	Object,						//다른 곳에서 그려준다. 투명화 등을 처리하기 위해 들고있어준다
};

class TileViewChild
	:public GameObject
{
	friend class TileView;
protected:
	TileView*				mcv_View;
	int						m_TileViewIndex;
	const TileViewType		m_TileViewType;
public:
	TileViewChild(TileView* view, TileViewType type);
	~TileViewChild();

	bool Initialize() override;
	void Reset() override;
	//void Update(float dt) override;
	void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	void PreRender();
	void PostRender();
	void Release() override;

	void ColorizeTile(const sf::Color& color, const CellIndex& tileIndex);
	//void NeedPriorityUpdate() { m_NeedPriorityUpdate = true; };

protected:
	void InitializeRaw();
	void InitializeTileObject();
	void ResetRaw();
	void ResetTileObject();

private:
	std::queue<CellIndex> m_ColorizedTiles;
	void ResetColorizedTile();

	void SetTileTransform(const sf::Transform& trans);
	//bool m_NeedPriorityUpdate=false;

protected:
	std::vector<std::vector<DTile*>> m_TileDrawable;
	//bool SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const;
};
