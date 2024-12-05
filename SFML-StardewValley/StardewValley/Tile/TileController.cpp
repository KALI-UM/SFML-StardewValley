#include "pch.h"
#include "Tile/TileController.h"
#include "Tile/TileMapSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/ButtonBar.h"
#include "TexCoordTable.h"
#include "DTile.h"
#include "TileObject.h"

TileController::TileController(TileMapSystem* sys, TileModel* model, TileView* view, int viewIndex)
	:m_TileMapSystem(sys), mcv_Model(model), mcv_View(view), m_ViewIndex(viewIndex)
{
}

TileController::~TileController()
{
}

bool TileController::Initialize()
{
	if (!mcv_Model || !mcv_View)return false;
	
	return true;
}

void TileController::Reset()
{
	if (m_ButtonBar)
		SetButtonTile(m_ButtonTexid, m_ButtonTileX, m_ButtonTileY);
}

void TileController::Update(float dt)
{
	m_MousePrevTile = m_MouseOverlaidTile;
	m_MouseOverlaidTile = mcv_View->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	if (m_MousePrevTile != m_MouseOverlaidTile)
		m_PrevTile = m_MousePrevTile;

	//std::cout << m_MouseOverlaidTile.x << "," << m_MouseOverlaidTile.y << std::endl;

	if (!UI_MGR->GetUIHasFocus())
	{
		switch (m_CurrStatus)
		{
		case ControlStatus::None:
			UpdateNone(dt);
			break;
		case ControlStatus::Place:
			UpdatePlace(dt);
			break;
		case ControlStatus::Destroy:
			UpdateDestroy(dt);
			break;
		case ControlStatus::TileType:
			UpdateType(dt);
			break;
		}

		if (INPUT_MGR->GetKeyDown(sf::Keyboard::LBracket))
		{
			GAME_MGR->SetViewZoom(m_ViewIndex, 2.0f);
		}
		else if (INPUT_MGR->GetKeyDown(sf::Keyboard::RBracket))
		{
			GAME_MGR->SetViewZoom(m_ViewIndex, 0.5f);
		}
		if (INPUT_MGR->GetKeyDown(sf::Keyboard::Up))
		{
			SetButtonTile(m_ButtonTexid, m_ButtonTileX, m_ButtonTileY - 1);
		}
		if (INPUT_MGR->GetKeyDown(sf::Keyboard::Down))
		{
			SetButtonTile(m_ButtonTexid, m_ButtonTileX, m_ButtonTileY + 1);
		}
		if (INPUT_MGR->GetKeyDown(sf::Keyboard::Left))
		{
			SetButtonTile(m_ButtonTexid, m_ButtonTileX - 1, m_ButtonTileY);
		}
		if (INPUT_MGR->GetKeyDown(sf::Keyboard::Right))
		{
			SetButtonTile(m_ButtonTexid, m_ButtonTileX + 1, m_ButtonTileY);
		}
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Right))
		{
			GAME_MGR->SetViewCenter(m_ViewIndex, INPUT_MGR->GetMouseViewPos(m_ViewIndex));
		}
	}
	else
	{
		UpdateNone(dt);
	}

}

void TileController::SetControlStatus(ControlStatus status)
{
	m_SelectingTiles.clear();
	m_CurrStatus = status;
}

void TileController::SetGridTextSize()
{
	//mcv_View->SetGridTextSize(m_Zoom);
}

void TileController::SetButtonBar(ButtonBar* bar)
{
	m_ButtonBar = bar;
	SetChildObj(bar, false);
}

void TileController::UpdateNone(float dt)
{
	//플레이모드
}

void TileController::UpdatePlace(float dt)
{
	if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	static bool isdraggingnow = false;
	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		if (isdraggingnow)
			m_TileMapSystem->BuildTilesById(m_SelectingTiles, m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit(), m_ButtonBar->GetCurrTexIds());
		m_SelectingTiles.clear();
		isdraggingnow = false;
		return;
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		SetNXMTiles(m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit(), m_DragStartTile);

		/*m_TileMapSystem->BuildTilesById(m_SelectingTiles, m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit(), m_ButtonBar->GetCurrTexIds());
		m_SelectingTiles.clear();*/
	}
	else if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
		{
			isdraggingnow = true;
			SetRangeIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile);
		}
	}
	else
	{
		mcv_View->ColorizeAllTile(ColorPalette::Gray, m_MouseOverlaidTile, m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit());
	}

}

void TileController::UpdateDestroy(float dt)
{
	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		m_TileMapSystem->BuildTilesById(m_SelectingTiles, "");
		m_SelectingTiles.clear();
		return;
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		SetNXMTiles(m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit(), m_DragStartTile);
	}
	else if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
		{
			SetRangeIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile);
		}
	}
}

void TileController::UpdateType(float dt)
{
	m_TileMapSystem->SetTileTypeMode();

	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		m_TileMapSystem->SetTilesType(m_SelectingTiles, m_CurrTileType);
		m_SelectingTiles.clear();
		return;
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		SetNXMTiles(m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit(), m_DragStartTile);
	}
	else if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
		{
			SetRangeIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile);
		}
	}
}

void TileController::UpdateDrag(float dt)
{

}

void TileController::SetButtonTile()
{
	auto tileres = TEXRESTABLE_MGR->GetTileTexRes(m_ButtonTexid);

	for (int j = 0; j < std::min(10, (int)tileres.children.size()); j++)
	{
		for (int i = 0; i < std::min(8, (int)tileres.children[j].size()); i++)
		{
			m_ButtonBar->SetButtonTex({ i,j }, tileres.children[m_ButtonTileY + j][m_ButtonTileX + i]);
		}
	}
}

void TileController::SetButtonTile(const std::string& texid, int x, int y)
{
	auto tileres = TEXRESTABLE_MGR->GetTileTexRes(texid);

	m_ButtonTileX = Utils::Clamp(x, 0, (int)tileres.children[0].size() - 8);
	m_ButtonTileY = Utils::Clamp(y, 0, (int)tileres.children.size() - 10);

	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			m_ButtonBar->SetButtonTex({ i,j }, tileres.children[m_ButtonTileY + j][m_ButtonTileX + i]);
		}
	}
}

void TileController::Set1x1Tile(const CellIndex& tileIndex, bool checkPossible)
{
	m_SelectingTiles.clear();
	if (checkPossible)
	{
		if (!mcv_Model->IsPossibleToSetTile(tileIndex, (int)m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
			return;
	}
	PushToSelectingTiles(tileIndex);
}

void TileController::SetLineIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible)
{
	m_SelectingTiles.clear();

	sf::Vector2i slope = startIndex - endIndex;
	sf::Vector2i currIndex = startIndex;
	int x0 = startIndex.x;
	int y0 = startIndex.y;
	int x1 = endIndex.x;
	int y1 = endIndex.y;

	//Bresenham's Line Algorithm
	//선의 기울기는 1과 0사이
	//x를 증가시킬 때마다 y 

	int dx = abs(slope.x);
	int dy = abs(slope.y);
	int sx = (startIndex.x < endIndex.x) ? 1 : -1;
	int sy = (startIndex.y < endIndex.y) ? 1 : -1;

	//slope err보정값
	int err = dx - dy;

	while (true)
	{
		if (checkPossible)
		{
			if (!mcv_Model->IsPossibleToSetTile(currIndex, (int)m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
				return;
		}

		// 현재 타일 색칠
		PushToSelectingTiles(currIndex);
		if (currIndex == endIndex)break;

		int e2 = 2 * err;
		bool needToMoveX = e2 > -dy;
		// x 방향으로 이동
		if (needToMoveX)
		{
			err -= dy;
			currIndex.x += sx;
		}

		bool needToMoveY = e2 < dx;
		// y 방향으로 이동
		if (needToMoveY)
		{
			err += dx;
			currIndex.y += sy;

			if (needToMoveX)
			{
				//PushToSelectingTiles({ currIndex.x - sx, currIndex.y });
				//PushToSelectingTiles({ currIndex.x, currIndex.y - sy });
			}
		}
	}
}

void TileController::SetRangeIntersectedTiles(const CellIndex& startIndex, const CellIndex& endIndex, bool checkPossible)
{
	m_SelectingTiles.clear();
	sf::Vector2i offset = (m_CurrStatus == ControlStatus::Place ? sf::Vector2i(m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit()) : sf::Vector2i(1, 1));

	for (int j = std::min(startIndex.y, endIndex.y); j <= std::max(startIndex.y, endIndex.y); j += offset.y)
	{
		for (int i = std::min(startIndex.x, endIndex.x); i <= std::max(startIndex.x, endIndex.x); i += offset.x)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToSetTile(currIndex, (int)m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
					continue;
			}
			PushToSelectingTiles(currIndex, m_CurrStatus == ControlStatus::Place ? m_ButtonBar->GetCurrButtonTile()->GetUnitxUnit() : sf::Vector2u(1, 1));
		}
	}
}

void TileController::SetNXMTiles(const sf::Vector2u& uu, const CellIndex& mouseIndex, bool checkPossible)
{
	m_SelectingTiles.clear();
	if (uu == sf::Vector2u(1, 1))
	{
		Set1x1Tile(mouseIndex, checkPossible);
		return;
	}

	for (int j = mouseIndex.y; j < mouseIndex.y + (int)uu.y; j++)
	{
		for (int i = mouseIndex.x; i < mouseIndex.x + (int)uu.x; i++)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToSetTile(currIndex, (int)m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
					return;
			}
		}
	}

	for (int j = mouseIndex.y; j < mouseIndex.y + (int)uu.y; j++)
	{
		for (int i = mouseIndex.x; i < mouseIndex.x + (int)uu.x; i++)
		{
			CellIndex currIndex = { i,j };
			PushToSelectingTiles(currIndex);
		}
	}
}

void TileController::PushToSelectingTiles(const CellIndex& tileIndex, const UNITxUNIT& uu)
{
	m_SelectingTiles.push_back(tileIndex);
	mcv_View->ColorizeAllTile(ColorPalette::Gray, tileIndex, uu);
}
