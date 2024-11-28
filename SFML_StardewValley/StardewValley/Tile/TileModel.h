#pragma once
#include "Tile/Tile.h"

//MCV-Model
//타일 데이터
//타일 이미지가 변경되면 뷰에게 알려야한다.
class TileController;
class TileModel
	:public GameObject
{
public:
	TileModel(const sf::Vector2u& cellCnt = { 100,100 }, const sf::Vector2f& cellSize = { 100, 100 });
	~TileModel();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	const sf::Vector2f m_CellSize;
	const sf::Vector2u m_CellCount;
	const int m_MaxLayer = (int)TileLayer::Max;

	void InitializeTileInfoLayer(const TileLayer& layer);

	void SetTileUpdateFunc(std::function<void(const TileLayer&, const CellIndex&)> func) { m_WhenNeedsToUpdateTileFunc = func; };
	//void SetTempEffectTileUpdateFunc(std::function<void(const CellIndex&)> func) { m_WhenNeedsToUpdateTempEffectTileFunc = func; };

	bool IsValidTileIndex(const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(const TileLayer& depth, const CellIndex& tileIndex) const;

	void SetTiles(const std::list<CellIndex>& tiles, const TileLayer& layer, const ID& id);
	void SetTile(const CellIndex& tileIndex, const TileLayer& layer, const ID& id, bool isTrueTile = true);
protected:
public:
	bool IsPossibleToPass(const CellIndex& tileIndex);
	bool IsPossibleToSetTile(const CellIndex& tileIndex, const TileLayer& layer, const ID& id);
protected:
	std::vector<std::vector<std::vector<TileInfo>>>		m_TileInfos;

	std::function<void(const TileLayer&, const CellIndex&)> m_WhenNeedsToUpdateTileFunc;
	void RequestUpdateTile(const TileLayer& layer, const CellIndex& tileIndex);
	//std::function<void(const CellIndex&)> m_WhenNeedsToUpdateTempEffectTileFunc;
	//void RequestTempEffectTile(const CellIndex& tileIndex);

	//void SetConnection(const CellIndex& tileIndex, const TileLayer& depth, TileType type);
	//void SetConnectionPowerline(const CellIndex& tileIndex, const TileLayer& depth);
	//std::string GetConnectedTileName(std::string& name, int connection);

	//int i = 0;
};

