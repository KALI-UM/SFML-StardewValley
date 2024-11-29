#pragma once
#include "Tile/Tile.h"

class TileModel;
class TileViewChild;
class TileGrid;
class DTile;
class TileView :
	public GameObject
{
protected:
	TileModel* mcv_Model;
	std::vector<TileViewChild*> m_LayerViews;
public:
	const TileModel* const GetModel()const { return mcv_Model; }
	const sf::Vector2f m_TileOffset = { 0,0 };
public:
	TileView(TileModel* model);
	~TileView();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	void PostRender();
	void Release() override;
	void SetTileLayerView(const TileLayer& layer, TileViewChild* child);
	void SetTileGrid(TileGrid* grid);
	void SetGridTextSize(float zoom);
	void SetTileLayerVisible(const TileLayer& layer, bool visible);

	void			SetTileTransform(const sf::Vector2f& zero, const sf::Transform& trans);
	sf::Transform	GetTileTransform() const { return m_TileTransform; }
	sf::Vector2f	GetTileCoordinatedPos(const sf::Vector2f& pos) const;
	CellIndex		GetTileCoordinatedIndex(const sf::Vector2f& pos, bool isTilepos = false) const;
	
	//sf::Vector2f	GetTileCoordinatedCenterPosByTileIndex(const CellIndex& tileIndex);
	//int				GetDrawableIndexByTileIndex(const CellIndex& tileIndex) const;

	void ColorizeTile(const sf::Color& color, const TileLayer& layer, const CellIndex& tileIndex);
	void ColorizeTile(const sf::Color& color, const TileLayer& layer, const std::list<CellIndex>& tiles);

protected:
	sf::Transform	m_TileTransform;
	TileGrid*		m_TileGrid;
	void PushToSpriteUpdateQue(const TileLayer& depth, const CellIndex& tileIndex);
	std::queue<std::pair<TileLayer, CellIndex>> m_SpriteUpdateQueue;


	void UpdateTileSprite();
};

