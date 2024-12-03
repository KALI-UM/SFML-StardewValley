#pragma once

#include "Tile/Tile.h"
class TileModel;

//게임용 오브젝트 시스템
class TileObjectSystem :
    public GameObject
{
protected:
	TileModel* mcv_Model;
public:
	TileObjectSystem(TileModel* model);
	~TileObjectSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;

	void LoadTileLayerRawFile();
	void LoadTileLayerObjectFile();

	bool IsPossibleToSetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex);
	void IsPossibleToPass();

	void SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);
	void RemoveTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);

protected:
	std::vector<std::list<TileObject*>>						m_TileObjects;

};

