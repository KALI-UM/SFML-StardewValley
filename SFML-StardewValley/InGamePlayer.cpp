#include "pch.h"
#include "InGamePlayer.h"
#include "Inventory.h"
#include "InGameEvent.h"

#include "Tile/TileObjectSystem.h"
#include "Tile/TileObject.h"

#include "TexCoordTable.h"
#include "Item/Item.h"


InGamePlayer::InGamePlayer(const std::string& name)
{
}

InGamePlayer::~InGamePlayer()
{
}

bool InGamePlayer::Initialize()
{
	m_PlayerBody = new DSprite();
	m_PlayerBody->SetPriorityType(DrawPriorityType::Custom);
	SetDrawableObj(m_PlayerBody);

	m_GetItemArea = new DRectangle(sf::FloatRect(0, 0, 20, 20), ColorPalette::Transparent, 1);
	m_GetItemArea->SetDebugDraw(false);
	m_GetItemArea->SetOrigin(OriginType::MC);
#ifdef _DEBUG
	m_GetItemArea->SetOutlineColor(ColorPalette::SkyBlue);
#endif // DEBUG
	SetDrawableObj(m_GetItemArea);
	return false;
}

void InGamePlayer::Reset()
{
	const TexRes& texres = TEXRESTABLE_MGR->GetTileTexRes("farmerbase.png#MoveDown");
	m_PlayerBody->SetTexture(texres.filepath);
	m_PlayerBody->SetTextureRect(TEXRESTABLE_MGR->GetTileTexRes(texres.children[0][0]).texcoord);
	m_PlayerBody->SetOrigin(OriginType::BC);

	m_Satmina = m_PlayerInfo.stamina;
	m_Inventory->PlayerReset();

	for (auto& item : m_PlayerInfo.inventory)
	{
		m_Inventory->PushItem(item.first, item.second);
	}
}

void InGamePlayer::Update(float dt)
{
	if (UI_HASFOCUS) return;
	m_TileIndex = m_TileSystem->GetTileCoordinatedTileIndex(GAME_MGR->GetScreenToViewPos(0, GAME_MGR->GetViewToScreenPos(1, getPosition())));


	if (m_TileSystem->IsInteractive(m_TileIndex))
	{
		INGAMEEVENT->EVENT(m_TileSystem->GetTileSubtypeByTileIndex(ViewLayer::Back, m_TileIndex));
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
	{
		for (int i = 0; i < 8; i++)
		{
			CellIndex check = m_TileIndex + Tile::d[i];
			TileObject* obj = m_TileSystem->GetTileObjectByTileIndex(ViewLayer::Object, check);
			if (obj && obj->IsToolInteractive())
			{
				obj->ToolInteraction();
			}
		}
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Right))
	{
		CellIndex currMouseTile = m_TileSystem->GetTileCoordinatedTileIndex(INPUT_MGR->GetMouseViewPos(0));
		if (m_TileSystem->IsInteractive(currMouseTile) && IsPlayerNearbyTile(currMouseTile))
		{
			INGAMEEVENT->EVENT(m_TileSystem->GetTileSubtypeByTileIndex(currMouseTile));
			return;
		}
	}

	m_Direction.x = INPUT_MGR->GetAxisRaw(Axis::Horizontal);
	m_Direction.y = INPUT_MGR->GetAxisRaw(Axis::Vertical);
	Utils::Normailize(m_Direction);

	if (m_Direction.x > 0)
	{
		CellIndex next = m_TileIndex + sf::Vector2i(1, 0);
		if (!m_TileSystem->IsPossibleToPass(next))
		{
			m_Direction.x = 0;
		}
	}
	else if (m_Direction.x < 0)
	{
		CellIndex next = m_TileIndex + sf::Vector2i(-1, 0);
		if (!m_TileSystem->IsPossibleToPass(next))
		{
			m_Direction.x = 0;
		}
	}
	if (m_Direction.y > 0)
	{
		CellIndex next = m_TileIndex + sf::Vector2i(0, 1);
		if (!m_TileSystem->IsPossibleToPass(next))
		{
			m_Direction.y = 0;
		}
	}
	else if (m_Direction.y < 0)
	{
		CellIndex next = m_TileIndex + sf::Vector2i(0, -1);
		if (!m_TileSystem->IsPossibleToPass(next))
		{
			m_Direction.y = 0;
		}
	}

	sf::Vector2f nextpos = getPosition() + m_Direction * Utils::Clamp(m_MoveSpeed * dt, 0.f, 16.f);
	//std::cout << nextTileIndex.x << "," << nextTileIndex.y << std::endl;
	setPosition(nextpos);
	m_PlayerBody->SetPriorityType(DrawPriorityType::Custom, getPosition().y - 8.0f);
}

void InGamePlayer::Exit()
{
	m_PlayerInfo.stamina = m_Satmina;
	m_PlayerInfo.inventory.clear();
	for (int i = 0; i < m_Inventory->GetInventoryMaxSize(); i++)
	{
		if (m_Inventory->GetInventoryData(i).first == nullptr)continue;

		m_PlayerInfo.inventory.push_back(
			{ m_Inventory->GetInventoryData(i).first->m_ItemId, m_Inventory->GetInventoryData(i).second });
	}
}

bool InGamePlayer::IsPlayerNearbyTile(const CellIndex& index)
{
	for (int i = 0; i < 8; i++)
	{
		CellIndex check = m_TileIndex + Tile::d[i];
		if (check == index)
		{
			return true;
		}
	}
	return false;
}

void InGamePlayer::SetInventory(Inventory* inventory)
{
	SetChildObj(inventory, false);
	m_Inventory = inventory;
}

bool InGamePlayer::GetItem(const std::string& itemId, int count) const
{
	return m_Inventory->PushItem(itemId, count);
}



