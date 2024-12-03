#pragma once
#include "Tile/Tile.h"

class DTile;

typedef std::string TILEOBJID;
typedef std::string SubType;
struct TileObjectData
{
	TILEOBJID	id;
	UNITxUNIT	uuSize;

	struct UnitData
	{
		CellIndex	offset;
		TEXID		texid;
		TileType	type;
		SubType		subtype;
	};

	std::vector<std::vector<UnitData>> tileTypeMap;
};

class TileObject :
	public GameObject
{
public:
	TileObject();
	~TileObject();

	bool Initialize() override;

	void LoadTileObject();

	DTile* GetDTile() const { return m_TileSprite; }
	const TileType& GetTileType(const CellIndex& tileIndex)const;
	const std::list<std::pair<CellIndex, TileType>>& GetTileTypes() const { return m_TileTypes; }
	std::list<std::pair<CellIndex, TileType>>	m_TileTypes;

protected:
	TileObjectData*											m_Data;
	CellIndex												m_TileIndex;
	CellIndex												m_TLOffset;

	DTile* m_TileSprite;
};

