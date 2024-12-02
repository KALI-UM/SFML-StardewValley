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

	void InitializeTileViewInfoLayer(const TileViewLayer& layer);
	void InitializeTileCollInfoLayer(const TileCollLayer& layer);

	void SetTileUpdateFunc(std::function<void(const TileViewLayer&, const CellIndex&)> func) { m_WhenNeedsToUpdateTileFunc = func; };
	void SetTileObjUpdateFunc(std::function<void(const TileViewLayer&, const CellIndex&, TileObject*)> func) { m_WhenNeedsToUpdateTileObjFunc = func; };
	void SetTileColorizeFunc(std::function<void(const sf::Color& color, const CellIndex&)> func) { m_WhenNeedsToColorizeTileFunc = func; };

	bool IsValidTileIndex(const CellIndex& tileIndex) const;
	const TileViewInfo& GetTileViewInfo(const TileViewLayer& layer, const CellIndex& tileIndex) const;
	const TileCollInfo& GetTileCollInfo(const TileCollLayer& layer, const CellIndex& tileIndex) const;

	void SetTiles(const std::list<CellIndex>& tiles, const TileViewLayer& layer, const TEXID& id);
	void SetTile(const CellIndex& tileIndex, const TileViewLayer& layer, const TEXID& id, bool isTrueTile = true);
	void SetCollisions(const std::list<CellIndex>& tiles, const TileCollLayer& layer, ColliderType type);
	void SetCollision(const CellIndex& tileIndex, const TileCollLayer& layer, ColliderType type);

	void CollisionTypeMode(const TileCollLayer& layer, ColliderType type);

protected:
public:
	bool IsPossibleToPass(const CellIndex& tileIndex);
	bool IsPossibleToSetTile(const CellIndex& tileIndex, const TileViewLayer& layer, const TEXID& id);
protected:
	std::vector<std::vector<std::vector<TileViewInfo>>>		m_TileViewInfos;
	std::vector<std::vector<std::vector<TileCollInfo>>>		m_TileCollInfos;

	std::function<void(const TileViewLayer&, const CellIndex&)> m_WhenNeedsToUpdateTileFunc;
	std::function<void(const TileViewLayer&, const CellIndex&, TileObject*)> m_WhenNeedsToUpdateTileObjFunc;
	std::function<void(const sf::Color& color, const CellIndex&)> m_WhenNeedsToColorizeTileFunc;
	void RequestUpdateTile(const TileViewLayer& layer, const CellIndex& tileIndex);
	void RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex);

	//std::function<void(const CellIndex&)> m_WhenNeedsToUpdateTempEffectTileFunc;
	//void RequestTempEffectTile(const CellIndex& tileIndex);

	//void SetConnection(const CellIndex& tileIndex, const TileLayer& depth, TileType type);
	//void SetConnectionPowerline(const CellIndex& tileIndex, const TileLayer& depth);
	//std::string GetConnectedTileName(std::string& name, int connection);

	//int i = 0;
};

