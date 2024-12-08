#pragma once
#include "Tile/Tile.h"

class DTile;
class TileObjectSystem;
struct TileObjData;
class TileObject :
	public GameObject, public ObjectPoolable
{
public:
	TileObject();
	~TileObject();

	bool IsNeedToReturn();
	void InitForObjectPool();
	void ResetForObjectPool();

	bool Initialize() override;
	void Reset() override;
	void LoadTileObject(const TOBJID& id);
	void SetTileSystem(TileObjectSystem* sys) { m_TileSystem = sys; }

	DTile* GetDTile() const { return m_TileSprite; }
	void SetTileIndex(const CellIndex& tileIndex) { m_TileIndex = tileIndex; }
	const TileType& GetTileTypeByTileIndex(const CellIndex& tileIndex)const;
	const std::string& GetTileSubtypeByTileIndex(const CellIndex& tileIndex)const;

	bool IsToolInteractive()const;
	bool IsPassableTileByTileIndex(const CellIndex& tileIndex)const;
	bool IsInteractiveTileByTileIndex(const CellIndex& tileIndex)const;

	const std::list<std::pair<CellIndex, TileType>>& GetTileTypes() const { return m_TileTypes; }

	void ToolInteraction();
protected:
	TOBJID											m_TObjId;
	const TileObjData* 								m_Data;
	CellIndex										m_TileIndex;
	std::list<std::pair<CellIndex, TileType>>		m_TileTypes;

	DTile*		m_TileSprite;
	int			m_HitCount;

	TileObjectSystem*								m_TileSystem;
};

