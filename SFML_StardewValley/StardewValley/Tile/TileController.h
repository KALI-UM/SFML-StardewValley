#pragma once
#include "Tile/Tile.h"

class TileMapSystem;
class TileModel;
class TileView;
enum class Action;
class ButtonBar;

enum class ControlStatus
{
	None,
	Place,
	Destroy,
	Drag,
};

class TileController
	:public GameObject
{
protected:
	int				m_ViewIndex;
	TileMapSystem*	m_TileMapSystem;
	TileModel*		mcv_Model;
	TileView*		mcv_View;
public:
	TileController(TileMapSystem* sys, TileModel* model, TileView* view, int viewIndex);
	~TileController();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixeUpdate(float dt) override;
	//void Release() override;

	void SetGridTextSize();
	void SetButtonBar(ButtonBar* bar);

	CellIndex GetMouseOverlaidTileIndex() const { return m_MouseOverlaidTile; }
	CellIndex GetDragStartTileIndex() const { return m_DragStartTile; }
	CellIndex GetMousePrevTileIndex() const { return m_MousePrevTile; }
	CellIndex GetPrevTileIndex() const { return m_PrevTile; }

	void UpdateNone(float dt);
	void UpdatePlace(float dt);
	void UpdateDestroy(float dt);
	void UpdateDrag(float dt);

	void SetButtonTile(int x, int y);
	void Set1x1Tile(const CellIndex& tileIndex, bool checkPossible = true);
	void SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible = true);
	void SetRangeIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible = true);
	void SetNXMTiles(const sf::Vector2u& lot, const CellIndex& centerIndex, bool checkPossible = true);

	void PushToSelectingTiles(const CellIndex& tileIndex);
protected:
	ControlStatus	m_CurrStatus = ControlStatus::Place;

	CellIndex		m_MouseOverlaidTile;
	CellIndex		m_MousePrevTile;
	CellIndex		m_DragStartTile;
	CellIndex		m_PrevTile;
	std::list<CellIndex> m_SelectingTiles;
	ButtonBar* m_ButtonBar;

	float			m_Zoom=10;
	int				m_ButtonTileX = 0;
	int				m_ButtonTileY = 0;
};

