#pragma once

typedef sf::Vector2i CellIndex;
typedef sf::Vector2u UNITxUNIT;
typedef std::string TEXID;
typedef std::string FILEPATH;
typedef sf::IntRect TEXRECT;
typedef std::string TOBJID;

//타일 레이어
enum class TileLayer
{
	Terrain,
	WaterEffect,
	Back,

	Object,
	Debug,

	Light,
	Max,
};

//오브젝트 레이어
enum class TileObjLayer
{
	Back,			//Terrain, water, and basic features (like permanent paths).
	Buildings,		//Placeholders for buildings (like the farmhouse). Any tiles placed on this layer will act like a wall unless the tile property has a "Passable" "T".
	Paths,			//Flooring, paths, grass, and debris (like stones, weeds, and stumps from the 'paths' tilesheet) which can be removed by the player.
	Front,			//Objects that are drawn on top of things behind them, like most trees. These objects will be drawn on top of the player if the player is North of them but behind the player if the player is south of them.
	AlwaysFront,	//Objects that are always drawn on top of other layers as well as the player. This is typically used for foreground effects like foliage cover.
	Max,
};

enum class TileEditorLayer
{
	Layer0,
	Layer1,
	Layer2,
	Layer3,
	Max,
};

enum class TileType
{
	PassableInteractive,
	Soil,			//경작가능땅
	Ground,			//그외 땅(마을, Floor 등...)

	ImpassableInteractive,
	Wall,			//벽(울타리 등, 물 외 못가는 곳)
	Water,			//물

	None,			//아무것도 없는 구간
};

class TileObject;
struct TileInfo
{
	CellIndex		index;

	TileObject*		owner = nullptr;
	TEXID			id;									
};

class Tile
{
public:
	//static std::string CollisionTypeToString(TileType type);
	//static TileType StringToCollisionType(const std::string& str);

	static std::string TileObjLayerToString(const TileObjLayer& layer);
	static TileObjLayer StringToTileObjLayer(const std::string& str);

	static std::string TileTypeToString(const TileType& type);
	static TileType StringToTileType(const std::string& str);

	static TileLayer TileObjLayerToTileLayer(const TileObjLayer& layer);
	static std::string TileLayerToString(const TileLayer& layer);
	static TileLayer StringToTileLayer(const std::string& str);

	static const CellIndex d[8];
};