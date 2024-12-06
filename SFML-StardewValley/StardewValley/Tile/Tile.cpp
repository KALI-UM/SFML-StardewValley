#include "pch.h"
#include "Tile/Tile.h"
#include "magic_enum.hpp"

const CellIndex Tile::d[8] = { {-1,0}, {1,0}, {0,-1},{0,1} ,{ -1,-1 },{1,-1},{-1,1},{1,1} };

//std::string Tile::CollisionTypeToString(TileType type)
//{
//	return std::string(magic_enum::enum_name(type).data());
//}
//
//TileType Tile::StringToCollisionType(const std::string& str)
//{
//	auto collision_str = magic_enum::enum_cast<TileType>(str);
//	if (collision_str.has_value())
//	{
//		return collision_str.value();
//	}
//	else
//	{
//		return TileType::None;
//	}
//}

std::string Tile::TileObjLayerToString(const TileObjLayer& layer)
{
	return std::string(magic_enum::enum_name(layer).data());
}

TileObjLayer Tile::StringToTileObjLayer(const std::string& str)
{
	auto layer_str = magic_enum::enum_cast<TileObjLayer>(str);
	if (layer_str.has_value())
	{
		return layer_str.value();
	}
	else
	{
		return TileObjLayer::Max;
	}
}

std::string Tile::TileTypeToString(const TileType& type)
{
	return std::string(magic_enum::enum_name(type).data());
}

TileType Tile::StringToTileType(const std::string& str)
{
	auto type_str = magic_enum::enum_cast<TileType>(str);
	if (type_str.has_value())
	{
		return type_str.value();
	}
	else
	{
		return TileType::Wall;
	}
}

TileLayer Tile::TileObjLayerToTileLayer(const TileObjLayer& layer)
{
	switch (layer)
	{
	case TileObjLayer::Back:
		return TileLayer::Back;
		break;
	case TileObjLayer::Buildings:
	case TileObjLayer::Paths:
	case TileObjLayer::Front:
		return TileLayer::Object;
		break;
	case TileObjLayer::AlwaysFront:
	case TileObjLayer::Max:
		return TileLayer::Front;
		break;
	default:
		return TileLayer::Object;
		break;
	}
}

std::string Tile::TileLayerToString(const TileLayer& layer)
{
	return std::string(magic_enum::enum_name(layer).data());
}

TileLayer Tile::StringToTileLayer(const std::string& str)
{
	auto layer_str = magic_enum::enum_cast<TileLayer>(str);
	if (layer_str.has_value())
	{
		return layer_str.value();
	}
	else
	{
		return TileLayer::Max;
	}
}
