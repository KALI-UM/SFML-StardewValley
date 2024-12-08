#pragma once
#include "Tile/Tile.h"

//MCV-Model
//타일 데이터
//타일 이미지가 변경되면 뷰에게 알려야한다.
class TileController;
class TileObject;
class TileModel
	:public GameObject
{
public:
	TileModel(int layermax, const sf::Vector2u& cellCnt = { 100,100 }, const sf::Vector2f& cellSize = { 100, 100 });
	~TileModel();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	const unsigned int m_LayerCnt;
	const sf::Vector2f m_CellSize;
	const sf::Vector2u m_CellCount;

	void InitializeTileInfos(int layer);

	void SetTileUpdateFunc(std::function<void(int, const CellIndex&)> func) { m_WhenNeedsToUpdateTileFunc = func; };
	//void SetTileObjUpdateFunc(std::function<void(const TileLayer&, const CellIndex&)> func) { m_WhenNeedsToUpdateTileObjFunc = func; };

	bool IsValidTileIndex(const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(int layer, const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(const ViewLayer& layer, const CellIndex& tileIndex) const;
	const TileInfo& GetTileInfo(const TileObjLayer& layer, const CellIndex& tileIndex) const;

	void SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);

	void SetTiles(int layer, const std::list<CellIndex>& tiles, const TEXID& id);
	void SetTile(int layer, const CellIndex& tileIndex, const TEXID& id, bool isTrueTile = true);

public:
	bool IsPossibleToSetTile(const CellIndex& tileIndex, int layer, const TEXID& id);
protected:
	std::vector<std::vector<std::vector<TileInfo>>>			m_TileInfos;
			

	std::function<void(int, const CellIndex&)> m_WhenNeedsToUpdateTileFunc;
	//std::function<void(const TileLayer& layer, const CellIndex&)> m_WhenNeedsToUpdateTileObjFunc;
	void RequestUpdateTile(int layer, const CellIndex& tileIndex);
	//void RequestUpdateTileObject(const TileLayer& layer, const CellIndex& tileIndex);

};

