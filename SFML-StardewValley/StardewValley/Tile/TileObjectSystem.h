#pragma once

#include "Tile/Tile.h"
class TileModel;
class TileView;
class Scene_InGame;
struct SceneRawFile;
//게임용 오브젝트 시스템
class TileObjectSystem :
    public GameObject
{
protected:
	TileModel*	mcv_Model;
	TileView*	mcv_View;
	Scene_InGame* m_CurrInGameScene;
public:
	TileObjectSystem(TileModel* model, TileView* view, Scene_InGame* scene);
	~TileObjectSystem();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	void PostRender() override;
	void Release();

	void SaveTileObjects(SceneRawFile* savedate);
	void LoadTileLayerRawFile(const std::string& terrainfile);

	CellIndex GetTileCoordinatedTileIndex(const sf::Vector2f& pos) const;
	TileObject* GetTileObjectByTileIndex(ViewLayer layer, const CellIndex& tileIndex) const;
	TileType	GetTileTypeByTileIndex(ViewLayer layer, const CellIndex& tileIndex) const;
	const std::string& GetTileSubtypeByTileIndex(const CellIndex& tileIndex) const;
	const std::string& GetTileSubtypeByTileIndex(ViewLayer layer, const CellIndex& tileIndex) const;

	bool IsPossibleToSetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex);
	bool IsPossibleToPass(const CellIndex& tileIndex) const;
	bool IsInteractive(const CellIndex& tileIndex) const;

	void ColorizePassableTile();
	void ColorizeInteractiveTile();
	void SetEffectLayerColor(const sf::Color& curr, const sf::Color& tar, float timer);
	void SetEffectLayerColor(const sf::Color& color);

	void SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);
	void SetTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, const std::string& objId);
	void RemoveTileObject(const TileObjLayer& layer, const CellIndex& tileIndex, TileObject* tileObj);


protected:
	std::vector<ObjectPool<TileObject>>	m_TileObjects;

	void SetTileColorizeFunc(std::function<void(const sf::Color&, int, const CellIndex&, bool)> func) { m_WhenNeedsToColorizeTileFunc = func; };
	void RequestColorizeTile(const sf::Color& color, int layer, const CellIndex& tileIndex, bool needReset);
	std::function<void(const sf::Color& color, int layer, const CellIndex&, bool)> m_WhenNeedsToColorizeTileFunc;


	inline static std::string empty="";

	sf::Color	m_CurrentColor;
	sf::Color	m_TargetColor;
	float m_EffectLayerTimer = 0.0f;
	float m_EffectLayerEndTime = 3.0f;
};

