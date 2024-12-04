#pragma once

#include "Tile/Tile.h"
class TileModel;
class TileView;

//게임용 오브젝트 시스템
class TileObjectSystem :
    public GameObject
{
protected:
	TileModel* mcv_Model;
	TileView*	mcv_View;
public:
	TileObjectSystem(TileModel* model, TileView* view);
	~TileObjectSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;

	void LoadTileLayerRawFile();
	void LoadTileLayerObjectFile();

	CellIndex GetTileCoordinatedTileIndex(const sf::Vector2f& pos) const;
	TileObject* GetTileObjectByTileIndex(TileLayer layer, const CellIndex& tileIndex) const;
	TileType	GetTileTypeByTileIndex(TileLayer layer, const CellIndex& tileIndex) const;

	bool IsPossibleToSetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex);
	bool IsPossibleToPass(const CellIndex& tileIndex);

	void ColorizePassableTile();


	void SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);
	void RemoveTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);


protected:
	std::vector<std::list<TileObject*>>						m_TileObjects;


	void SetTileColorizeFunc(std::function<void(const sf::Color& color, const CellIndex&)> func) { m_WhenNeedsToColorizeTileFunc = func; };
	void RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex);
	std::function<void(const sf::Color& color, const CellIndex&)> m_WhenNeedsToColorizeTileFunc;
};

