#pragma once

typedef sf::Vector2i CellIndex;
typedef sf::Vector2u LOT;
typedef std::string TEXID;
typedef std::string FILEPATH;
typedef sf::IntRect TEXRECT;

struct TexRes
{
    TEXID id;
    FILEPATH filepath;
    LOT lotSize;
    CellIndex index;
    TEXRECT texcoord;
    std::vector<std::vector<TEXID>> children;
};

class TexCoordTable :
    public DataTable
{
public:
    TexCoordTable();
    ~TexCoordTable();

    bool Load() override;
    void Release() override;

    const TEXRECT& GetTexIntRect(const TEXID& id) const;
    const sf::FloatRect& GetTexFloatRect(const TEXID& id) const;
    const TexRes& GetTileTexRes(const TEXID& id) const;

protected:
    std::unordered_map<TEXID, TexRes> m_TileTexCoord;
    static TexRes m_Empty;

    std::string m_TileFilePath = "graphics/";
   sf::Vector2i Get2Number(const std::string& str, const std::string& separator);
};

