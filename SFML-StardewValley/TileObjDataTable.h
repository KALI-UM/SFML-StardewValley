#pragma once
#include "Tile/Tile.h"

typedef std::string SubType;

namespace sf
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2f, x, y);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2i, x, y);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2u, x, y);
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::IntRect, left, top, width, height);
}

struct TileObjRawData
{
	TOBJID	id;
	UNITxUNIT	uuSize;
	CellIndex	originIndex;
	int			priorityY;
	struct UnitData
	{
		TEXID		texid;
		std::string	type;
		std::string	subtype;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(UnitData, texid, type, subtype)
	};

	std::vector<std::vector<UnitData>> tileTypeMap;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TileObjRawData, id, uuSize, originIndex, priorityY, tileTypeMap)
};

struct TexRes;
struct TileObjData
{
	TOBJID	id;
	UNITxUNIT	uuSize;
	CellIndex	originIndex;
	int			priorityY;
	struct UnitData
	{
		const TexRes*		texres;
		TileType			type;
		SubType				subtype;
	};

	std::vector<std::vector<UnitData>> tileTypeMap;
};


class TileObjDataTable :
    public DataTable
{
public:
    TileObjDataTable();
    ~TileObjDataTable();

    bool Load() override;
    void Release() override;

    const TileObjData& GetTileObjectData(const TOBJID& id) const;

protected:
	void LoadTileObjFile(TileObjData& data, const std::string& filepath);
	void LoadTileObjRawFile(TileObjRawData& data, const std::string& filepath);

    std::unordered_map<TOBJID, TileObjData>	m_TileObjectDatas;
    static TileObjData						m_Empty;

};

