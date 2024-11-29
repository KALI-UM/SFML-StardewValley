#pragma once
#include "Tile/Tile.h"

class TileView;
class DTile;
class TileViewChild
	:public GameObject
{
	friend class TileView;
protected:
	TileView* mcv_View;
	TileLayer m_Layer;
public:
	TileViewChild(TileView* view);
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
private:
	std::queue<CellIndex> m_ColorizedTiles;
	void ResetColorizedTile();

	void SetTileTransform(const sf::Transform& trans);
	//bool m_NeedPriorityUpdate=false;

protected:
	std::vector<std::vector<DTile*>> m_TileDrawable;
	//bool SortTile(const DrawableObject* dobj1, const DrawableObject* dobj2) const;
};
