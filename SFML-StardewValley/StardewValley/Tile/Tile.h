#pragma once

typedef sf::Vector2i CellIndex;
typedef sf::Vector2u LOT;
typedef std::string TEXID;
typedef std::string FILEPATH;
typedef sf::IntRect TEXRECT;

enum class TileLayer
{
	Terrain,		//Terrain, water, 
	Back,			//and basic features (like permanent paths).
	Buildings,		//Placeholders for buildings (like the farmhouse). Any tiles placed on this layer will act like a wall unless the tile property has a "Passable" "T".
	Paths,			//Flooring, paths, grass, and debris (like stones, weeds, and stumps from the 'paths' tilesheet) which can be removed by the player.
	Front,			//Objects that are drawn on top of things behind them, like most trees. These objects will be drawn on top of the player if the player is North of them but behind the player if the player is south of them.
	AlwaysFront,	//Objects that are always drawn on top of other layers as well as the player. This is typically used for foreground effects like foliage cover.
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
	TEXID			id;
	LOT	lotSize = {1,1};
	CollisionType	collision;
};


class Tile
{
public:
	static std::string CollisionTypeToString(CollisionType type);
	static CollisionType StringToCollisionType(const std::string& str);
	static std::string TileLayerToString(TileLayer layer);
	static TileLayer StringToTileLayer(const std::string str);

	static const CellIndex d[8];

};