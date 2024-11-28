#pragma once
#include "Tile/Tile.h"

class TileTexCoordTable :
    public DataTable
{
public:
    TileTexCoordTable();
    ~TileTexCoordTable();

    bool Load() override;
    void Release() override;

    const TEXRECT& GetTexIntRect(const ID& id) const;
    const sf::FloatRect& GetTexFloatRect(const ID& id) const;
    const TileTexRes& GetTileTexRes(const ID& id) const;

protected:
    std::unordered_map<ID, TileTexRes> m_TileTexCoord;
    static TileTexRes m_Empty;

    std::string m_TileFilePath = "tile/";
   sf::Vector2i Get2Number(const std::string& str, const std::string& separator);
};

