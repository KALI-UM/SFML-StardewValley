#pragma once
#include "Tile/Tile.h"

class TileModel;

//맵배치용 시스템
class TileMapSystem :
	public GameObject
{
protected:
	TileModel* mcv_Model;
public:
	TileMapSystem(TileModel* model);
	~TileMapSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;

	void SetTileColorizeFunc(std::function<void(const sf::Color& color, const CellIndex&)> func) { m_WhenNeedsToColorizeTileFunc = func; };

	void LoadTileViewRawFile(TileEditorLayer layer);
	void SaveTileViewRawFile(TileEditorLayer layer);

	void LoadTileTypeFile();
	void SaveTileTypeFile();

	void SaveAsObjectData(const std::string& tileObjId, const std::string& texfilepath, const std::string& typefilepath);

	void BuildTilesById(const std::list<CellIndex> tiles, const UNITxUNIT& uu, const std::list <TEXID>& ids);
	void BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id);
	void BuildTilesById(const CellIndex& tile, const TEXID& id);

	void SetTilesType(const std::list<CellIndex>& tiles, TileType type);
	void SetTileType(const CellIndex& tileIndex, TileType type);
	void SetTileTypeMode(TileType type);


protected:

	TEXID	m_CurrTileTexId;
	TileEditorLayer m_CurrLayer = TileEditorLayer::Layer0;

	std::vector<std::vector<TileType>>		m_TileTypeInfos;
	
	std::function<void(const sf::Color& color, const CellIndex&)> m_WhenNeedsToColorizeTileFunc;
	void RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex);


	//std::list<std::pair<CellIndex, TEXID>>				m_PrevTileId;				//되돌리기를 위해 저장한다

public:
	void SetCurrTileLayer(TileEditorLayer layer) { m_CurrLayer = layer; };
	const TileEditorLayer& GetCurrTileLayer()const { return m_CurrLayer; }
	void SetCurrId(const TEXID& id) { m_CurrTileTexId = id; }
	const TEXID& GetCurrId() const { return m_CurrTileTexId; }
};


