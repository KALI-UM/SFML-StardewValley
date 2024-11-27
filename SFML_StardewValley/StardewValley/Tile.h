#pragma once

typedef sf::Vector2i CellIndex;
typedef std::string ID;
typedef std::string FILENAME;
typedef sf::FloatRect TEXRECT;

struct TileTexRes
{
	ID id;
	FILENAME filepath;
	TEXRECT texcoord;

	std::vector<ID> children;
};

enum class TileLayer
{
	Back,			//Terrain, water, and basic features (like permanent paths).
	Buildings,		//Placeholders for buildings (like the farmhouse). Any tiles placed on this layer will act like a wall unless the tile property has a "Passable" "T".
	Paths,			//Flooring, paths, grass, and debris (like stones, weeds, and stumps from the 'paths' tilesheet) which can be removed by the player.
	Front,			//Objects that are drawn on top of things behind them, like most trees. These objects will be drawn on top of the player if the player is North of them but behind the player if the player is south of them.
	AlwaysFront,		//Objects that are always drawn on top of other layers as well as the player. This is typically used for foreground effects like foliage cover.
	Max,
};


struct TileInfo
{
	CellIndex		index;
	CellIndex		ower;
	ID				id;
	sf::Vector2u	lotSize = {1,1};
	bool passable;
};

//
//class Tile
//{
//public:
//	static void Initialize();
//
//	static std::string GetTypeToString(TileType type);
//	static TileType GetTypeToEnum(const std::string& type);
//
//	static std::string GetZoneToString(ZoneType type);
//	static ZoneType GetZoneToEnum(const std::string& type);
//	static std::string GetZoneToName(ZoneType type);
//	static ZoneType GetNameToZone(const NAME& name);
//
//	static const CellIndex d[8];
//
//	static void SetLotSet();
//	static std::vector<std::list<CellIndex>> lotSet;
//};