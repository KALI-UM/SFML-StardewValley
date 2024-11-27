#pragma once
#include "Tile.h"

class TileTexCoordTable :
    public DataTable
{
public:
    TileTexCoordTable();
    ~TileTexCoordTable();

    bool Load() override;
    void Release() override;

    const sf::IntRect& GetTexIntRect(const ID& id) const;
    const sf::FloatRect& GetTexFloatRect(const ID& id) const;
    const TileTexRes& GetTileTexRes(const ID& id) const;

protected:
    std::unordered_map<ID, TileTexRes> m_TileTexCoord;
    static TileTexRes m_Empty;

    std::string m_TileFilePath = "tile/";
};

