#include "pch.h"
#include "Tile/Tile.h"
#include "magic_enum.hpp"

const CellIndex Tile::d[8] = { {-1,0}, {1,0}, {0,-1},{0,1} ,{ -1,-1 },{1,-1},{-1,1},{1,1} };

std::string Tile::CollisionTypeToString(CollisionType type)
{
	return std::string(magic_enum::enum_name(type).data());
}

CollisionType Tile::StringToCollisionType(const std::string& str)
{
	auto collision_str = magic_enum::enum_cast<CollisionType>(str);
	if (collision_str.has_value())
	{
		return collision_str.value();
	}
	else
	{
		return CollisionType::Passable;
	}
}

std::string Tile::TileViewLayerToString(TileViewLayer layer)
{
	return std::string(magic_enum::enum_name(layer).data());
}

TileViewLayer Tile::StringToTileViewLayer(const std::string str)
{
	auto layer_str = magic_enum::enum_cast<TileViewLayer>(str);
	if (layer_str.has_value())
	{
		return layer_str.value();
	}
	else
	{
		return TileViewLayer::Terrain;
	}
}
