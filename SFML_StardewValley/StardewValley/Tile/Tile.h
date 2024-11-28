#pragma once

typedef sf::Vector2i CellIndex;

typedef sf::Vector2u LOT;
typedef std::string ID;
typedef std::string FILEPATH;
typedef sf::IntRect TEXRECT;

struct TileTexRes
{
	ID id;
	FILEPATH filepath;
	LOT lotSize;
	CellIndex index;
	TEXRECT texcoord;
	std::vector<std::vector<ID>> children;
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

enum class CollisionType
{
	Block,
	Enter,
	None,
};

struct TileInfo
{
	CellIndex		index;
	CellIndex		ower;
	ID				id;
	LOT	lotSize = {1,1};
	CollisionType	collision;
};


class Tile
{
public:

	static std::string GetCollisionTypeToString(CollisionType type);
	static CollisionType GetStringToCollisionType(const std::string& str);
	static std::string GetTileLayerToString(TileLayer layer);
	static TileLayer GetStringToTileLayer(const std::string str);

	static const CellIndex d[8];

};