#pragma once
#include "Tile/Tile.h"
#include <imgui.h>

class DTile;


struct TileObjData;
class TileObject :
	public GameObject
{
public:
	TileObject(const TOBJID& id);
	~TileObject();

	bool Initialize() override;
	void Reset() override;
	void LoadTileObject();

	DTile* GetDTile() const { return m_TileSprite; }
	void SetTileIndex(const CellIndex& tileIndex) { m_TileIndex = tileIndex; }
	const TileType& GetTileTypeByTileIndex(const CellIndex& tileIndex)const;
	bool IsPassableTileByTileIndex(const CellIndex& tileIndex)const;
	bool IsInteractiveTileByTileIndex(const CellIndex& tileIndex)const;

	const std::list<std::pair<CellIndex, TileType>>& GetTileTypes() const { return m_TileTypes; }
	std::list<std::pair<CellIndex, TileType>>	m_TileTypes;

protected:
	const TOBJID											m_TOBjId;
	const TileObjData*										m_Data;
	CellIndex												m_TileIndex;

	DTile* m_TileSprite;
};

