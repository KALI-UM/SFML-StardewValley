#include "pch.h"
#include "Tile/TileController.h"
#include "Tile/TileMapSystem.h"
#include "Tile/TileModel.h"
#include "Tile/TileView.h"
#include "Tile/ButtonBar.h"
#include "TexCoordTable.h"

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

	return false;
}

void TileController::Reset()
{
	SetButtonTile(m_ButtonTileX, m_ButtonTileY);
}

void TileController::Update(float dt)
{
	m_MousePrevTile = m_MouseOverlaidTile;
	m_MouseOverlaidTile = mcv_View->GetTileCoordinatedIndex(INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	if (m_MousePrevTile != m_MouseOverlaidTile)
		m_PrevTile = m_MousePrevTile;

	std::cout << m_MouseOverlaidTile.x << "," << m_MouseOverlaidTile.y << std::endl;

	if (!m_ButtonBar->GetHasFocus())
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
		case ControlStatus::Drag:
			UpdateDrag(dt);
			break;
		}
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
		SetButtonTile(m_ButtonTileX, m_ButtonTileY - 1);
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::Down))
	{
		SetButtonTile(m_ButtonTileX, m_ButtonTileY + 1);
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::Left))
	{
		SetButtonTile(m_ButtonTileX - 1, m_ButtonTileY);
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::Right))
	{
		SetButtonTile(m_ButtonTileX + 1, m_ButtonTileY);
	}
	if (INPUT_MGR->GetMouseDown(sf::Mouse::Right))
	{
		GAME_MGR->SetViewCenter(m_ViewIndex, INPUT_MGR->GetMouseViewPos(m_ViewIndex));
	}
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
	/*if (m_TileMapSystem->GetCurrAction() == Action::Move)
	{
		if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
		{
			GAME_MGR->SetViewCenter(m_ViewIndex, INPUT_MGR->GetMouseViewPos(m_ViewIndex));
		}
	}*/
}

void TileController::UpdatePlace(float dt)
{
	if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		m_DragStartTile = m_MouseOverlaidTile;
		SetNXMTiles({1,1}, m_DragStartTile);

		m_CurrStatus = ControlStatus::Drag;
	}
	else
	{
		mcv_View->ColorizeTile(ColorPalette::Gray, m_TileMapSystem->GetCurrTileLayer(), m_MouseOverlaidTile);
	}
}

void TileController::UpdateDestroy(float dt)
{
	/*if (!mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))return;

	if (INPUT_MGR->GetMouse(sf::Mouse::Left))
	{
		if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left))
			mcv_Model->SetTempEffectTiles(m_MousePrevTile, TileType::Other, "", "bulldozer_1");

		if (m_MousePrevTile != m_MouseOverlaidTile)
		{
			Set1x1Tile(m_MouseOverlaidTile);
			if (!m_SelectingTiles.empty())
			{
				m_TileMapSystem->DestroySomething(m_SelectingTiles.front());
			}
		}
	}

	mcv_View->ColorizeTile(ColorPalette::Gray, m_TileMapSystem->GetCurrTileSet().lotSize, m_MouseOverlaidTile);*/
}

void TileController::UpdateDrag(float dt)
{
	if (INPUT_MGR->GetMouseUp(sf::Mouse::Left))
	{
		m_TileMapSystem->BuildTilesById(m_SelectingTiles, m_TileMapSystem->GetCurrId());
		m_SelectingTiles.clear();
		m_CurrStatus = ControlStatus::Place;
		return;
	}

	if (INPUT_MGR->GetMouseDrag(sf::Mouse::Left) && m_DragStartTile != m_MouseOverlaidTile)
	{
		if (mcv_Model->IsValidTileIndex(m_DragStartTile) && mcv_Model->IsValidTileIndex(m_MouseOverlaidTile))
		{
			SetRangeIntersectedTiles(m_DragStartTile, m_MouseOverlaidTile, false);
		}
	}
}

/*
void TileController::SetCurrButton(Action btt)
{
	switch (btt)
	{
	case Action::Bulldozer:
		SetCurrButton(Action::DestroyNormal);
		return;
		break;
	case Action::Landscape:
	case Action::Dispatch:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::PowerSupply:
		SetCurrButton(Action::PowerLine);
		return;
		break;
	case Action::WaterSupply:
	case Action::Religion:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::Road:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Rail:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Port:
	case Action::ZoneResidential:
	case Action::ZoneCommercial:
	case Action::ZoneIndustrial:
		m_CurrStatus = ControlStatus::Place;
		break;
	case Action::Education:
		SetCurrButton(Action::GradeSchool);
		return;
	case Action::PublicService:
		SetCurrButton(Action::PoliceStation);
		return;
		return;
	case Action::Recreation:
		SetCurrButton(Action::Park);
		return;
		break;
	case Action::ZoomIn:
		GAME_MGR->SetViewZoom(m_ViewIndex, 0.5f);
		m_CurrStatus = ControlStatus::None;
		return;
		break;
	case Action::ZoomOut:
		GAME_MGR->SetViewZoom(m_ViewIndex, 2.0f);
		m_CurrStatus = ControlStatus::None;
		return;
		break;
	case Action::Move:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::NotUse18:
	case Action::NotUse19:
	case Action::NotUse20:
	case Action::NotUse21:
	case Action::NotUse22:
	case Action::NotUse23:
	case Action::NotUse24:
	case Action::NotUse25:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::DestroyNormal:
	case Action::DestroyZone:
		m_CurrStatus = ControlStatus::Destroy;
		break;
	case Action::Trees:
	case Action::Water:
		m_CurrStatus = ControlStatus::None;
		break;
	case Action::PowerLine:
	case Action::PowerPlant:
	case Action::GradeSchool:
	case Action::College:
	case Action::Library:
	case Action::PoliceStation:
	case Action::FireStation:
	case Action::Hospital:
	case Action::Park:
	case Action::LargePark:
	case Action::Zoo:
	case Action::Stadium:
		m_CurrStatus = ControlStatus::Place;
		break;
	}

	SetCurrCusor(btt);
	m_TileMapSystem->SetCurrTileSet(btt);
}

void TileController::SetCurrCusor(Action btt)
{
	switch (btt)
	{
	case Action::Bulldozer:
	case Action::DestroyNormal:
	case Action::DestroyZone:
		m_Cursor->SetCursorMode(Action::Bulldozer);
		break;
	case Action::Landscape:
	case Action::Trees:
	case Action::Water:
		m_Cursor->SetCursorMode(Action::Landscape);
		break;
	case Action::Dispatch:
		break;
	case Action::PowerSupply:
	case Action::PowerLine:
	case Action::PowerPlant:
		m_Cursor->SetCursorMode(Action::PowerSupply);
		break;
	case Action::Education:
	case Action::GradeSchool:
	case Action::College:
	case Action::Library:
		m_Cursor->SetCursorMode(Action::Education);
		break;
	case Action::PublicService:
	case Action::PoliceStation:
	case Action::FireStation:
	case Action::Hospital:
		m_Cursor->SetCursorMode(Action::PublicService);
		break;
	case Action::Recreation:
	case Action::Park:
	case Action::LargePark:
	case Action::Zoo:
	case Action::Stadium:
		m_Cursor->SetCursorMode(Action::Recreation);
		break;
	default:
		m_Cursor->SetCursorMode(btt);
		break;
	}
}*/

void TileController::SetButtonTile(int x, int y)
{
	m_ButtonTileX = Utils::Clamp(x, 0, 41-8);
	m_ButtonTileY = Utils::Clamp(y, 0, 56-8);

	std::string buttonid = "OutdoorsSpring.png#Spring";
	auto tileres = TEXRESTABLE_MGR->GetTileTexRes(buttonid);

	for (int i = (int)Action::SetTile0; i < (int)Action::Max; i++)
	{
		int index = i - (int)Action::SetTile0;
		m_ButtonBar->SetButtonTex(tileres.filepath, (Action)(i), TEXRESTABLE_MGR->GetTexIntRect(tileres.children[m_ButtonTileY + index / 8][m_ButtonTileX + index % 8]));
		m_ButtonBar->SetButtonFunc((Action)(i), std::bind(&TileMapSystem::SetCurrId, m_TileMapSystem, tileres.children[m_ButtonTileY + index / 8][m_ButtonTileX + index % 8]));
	}
}

void TileController::Set1x1Tile(const CellIndex& tileIndex, bool checkPossible)
{
	m_SelectingTiles.clear();
	if (checkPossible)
	{
		if (!mcv_Model->IsPossibleToSetTile(tileIndex, m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
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
			if (!mcv_Model->IsPossibleToSetTile(currIndex, m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
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

	for (int j = std::min(startIndex.y, endIndex.y); j <= std::max(startIndex.y, endIndex.y); j++)
	{
		for (int i = std::min(startIndex.x, endIndex.x); i <= std::max(startIndex.x, endIndex.x); i++)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToSetTile(currIndex, m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
					continue;
			}
			PushToSelectingTiles(currIndex);
		}
	}
}

void TileController::SetNXMTiles(const sf::Vector2u& lot, const CellIndex& centerIndex, bool checkPossible)
{
	m_SelectingTiles.clear();

	CellIndex startIndex;
	if (lot == sf::Vector2u(1, 1))
	{
		Set1x1Tile(centerIndex, checkPossible);
		return;
	}
	else if (lot == sf::Vector2u(2, 2))
	{
		startIndex = centerIndex + sf::Vector2i(-1, 0);
	}
	else if (lot == sf::Vector2u(3, 3))
	{
		startIndex = centerIndex + sf::Vector2i(-1, -1);
	}
	else if (lot == sf::Vector2u(4, 4))
	{
		startIndex = centerIndex + sf::Vector2i(-2, -1);
	}

	for (int j = startIndex.y; j < startIndex.y + (int)lot.y; j++)
	{
		for (int i = startIndex.x; i < startIndex.x + (int)lot.x; i++)
		{
			CellIndex currIndex = { i,j };
			if (checkPossible)
			{
				if (!mcv_Model->IsPossibleToSetTile(currIndex, m_TileMapSystem->GetCurrTileLayer(), m_TileMapSystem->GetCurrId()))
					return;
			}

		}
	}

	for (int j = startIndex.y; j < startIndex.y + (int)lot.y; j++)
	{
		for (int i = startIndex.x; i < startIndex.x + (int)lot.x; i++)
		{
			CellIndex currIndex = { i,j };
			PushToSelectingTiles(currIndex);
		}
	}
}

void TileController::PushToSelectingTiles(const CellIndex& tileIndex)
{
	m_SelectingTiles.push_back(tileIndex);
	mcv_View->ColorizeTile(ColorPalette::Gray, m_TileMapSystem->GetCurrTileLayer(), tileIndex);
}
