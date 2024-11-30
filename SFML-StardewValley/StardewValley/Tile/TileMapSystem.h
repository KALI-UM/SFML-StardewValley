#pragma once
#include "Tile/Tile.h"

enum class Action
{
	CollisionType0,
	CollisionType1,
	CollisionType2,
	CollisionEnd0,
	CollisionEnd1,
	CollisionEnd2,
	CollisionEnd3,
	CollisionEnd4,

	SetTile0,
	SetTile1,
	SetTile2,
	SetTile3,
	SetTile4,
	SetTile5,
	SetTile6,
	SetTile7,
	SetTile8,
	SetTile9,
	SetTile10,
	SetTile11,
	SetTile12,
	SetTile13,
	SetTile14,
	SetTile15,
	SetTile16,
	SetTile17,
	SetTile18,
	SetTile19,
	SetTile20,
	SetTile21,
	SetTile22,
	SetTile23,
	SetTile24,
	SetTile25,
	SetTile26,
	SetTile27,
	SetTile28,
	SetTile29,
	SetTile30,
	SetTile31,
	SetTile32,
	SetTile33,
	SetTile34,
	SetTile35,
	SetTile36,
	SetTile37,
	SetTile38,
	SetTile39,
	SetTile40,
	SetTile41,
	SetTile42,
	SetTile43,
	SetTile44,
	SetTile45,
	SetTile46,
	SetTile47,
	SetTile48,
	SetTile49,
	SetTile50,
	SetTile51,
	SetTile52,
	SetTile53,
	SetTile54,
	SetTile55,
	SetTile56,
	SetTile57,
	SetTile58,
	SetTile59,
	SetTile60,
	SetTile61,
	SetTile62,
	SetTile63,
	SetTile64,
	SetTile65,
	SetTile66,
	SetTile67,
	SetTile68,
	SetTile69,
	SetTile70,
	SetTile71,
	SetTile72,
	SetTile73,
	SetTile74,
	SetTile75,
	SetTile76,
	SetTile77,
	SetTile78,
	SetTile79,


	Max,
};

namespace std {
	template <> struct hash<Action> {
		size_t operator() (const Action& t) const { return size_t(t); }
	};
}

class TileModel;
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

	void BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id);
	void BuildTilesById(const CellIndex& tile, const TEXID& id);

	void LoadTileLayerFile(TileLayer layer);
	void SaveTileLayerFile(TileLayer layer);

protected:
	void InitializeActionSet();

	Action m_CurrAction;
	TEXID	m_CurrTileTexId;
	TileLayer m_CurrLayer;
	//std::list<std::pair<CellIndex, TEXID>>				m_PrevTileId;				//되돌리기를 위해 저장한다

public:
	void SetCurrTileLayer(TileLayer layer) { m_CurrLayer = layer; };
	const TileLayer& GetCurrTileLayer()const { return m_CurrLayer; }
	void SetCurrAction(Action action) { m_CurrAction = action; };
	const Action& GetCurrAction() const { return m_CurrAction; }
	void SetCurrId(const TEXID& id) { m_CurrTileTexId = id; }
	const TEXID& GetCurrId() const { return m_CurrTileTexId; }
};


