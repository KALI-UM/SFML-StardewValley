#include "pch.h"
#include "Tile/Tile.h"
#include "magic_enum.hpp"

const CellIndex Tile::d[8] = { {-1,0}, {1,0}, {0,-1},{0,1} ,{ -1,-1 },{1,-1},{-1,1},{1,1} };

std::string Tile::GetCollisionTypeToString(CollisionType type)
{
	return std::string(magic_enum::enum_name(type).data());
}

CollisionType Tile::GetStringToCollisionType(const std::string& str)
{
	auto collision_str = magic_enum::enum_cast<CollisionType>(str);
	if (collision_str.has_value())
	{
		return collision_str.value();
	}
	else
	{
		return CollisionType::None;
	}
}

std::string Tile::GetTileLayerToString(TileLayer layer)
{
	return std::string(magic_enum::enum_name(layer).data());
}

TileLayer Tile::GetStringToTileLayer(const std::string str)
{
	auto layer_str = magic_enum::enum_cast<TileLayer>(str);
	if (layer_str.has_value())
	{
		return layer_str.value();
	}
	else
	{
		return TileLayer::Back;
	}
}
