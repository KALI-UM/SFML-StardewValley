#include "pch.h"
#include "Inventory.h"
#include "Item/Item.h"
#include "ItemDataTable.h"
#include "ItemGenerator.h"
#include "InventoryUI.h"


Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

bool Inventory::Initialize()
{
	m_Inventory.resize(m_MaxSize);	
	return false;
}

void Inventory::Reset()
{
}

void Inventory::PlayerReset()
{
	for (auto& inven : m_Inventory)
	{
		inven = { nullptr, 0 };
	}
	m_UI->PlayerReset();
}

void Inventory::Update(float dt)
{
	for (int numkey = (int)sf::Keyboard::Num1; numkey <= sf::Keyboard::Num9; numkey++)
	{
		if (INPUT_MGR->GetKeyDown((sf::Keyboard::Key)numkey))
		{
			SetCurrentIndex((sf::Keyboard::Key)numkey - (int)sf::Keyboard::Num1);
		}
	}

	if (INPUT_MGR->GetKeyDown(sf::Keyboard::Num0))
		SetCurrentIndex(9);
}

bool Inventory::PushItem(const ITEMID& item, int count)
{
	const ItemDataRes& itemdata = ITEMDATATABLE_MGR->GetItemDataRes(item);

	int lastempty = m_MaxSize;
	if (Item::GetStackTypeByItemType(itemdata.type) == StackType::Stackable)
	{
		for (int i = 0; i < m_Inventory.size(); i++)
		{
			if (m_Inventory[i].second != 0)
			{
				if (m_Inventory[i].first->m_ItemId == itemdata.id)
				{
					m_Inventory[i].second += count;
					m_UI->SetItemIcon(i, itemdata.iconTexId, m_Inventory[i].second);
					return true;
				}
			}
			else
			{
				lastempty = std::min(lastempty, i);
			}
		}
	}

	if (lastempty != m_MaxSize)
	{
		m_Inventory[lastempty] = { ITEMGENERATOR->GetItem(item), count };
		m_UI->SetItemIcon(lastempty, itemdata.iconTexId, count);

		return true;
	}
	else
	{
		return false;
	}
}

void Inventory::RemoveItem(const ITEMID& item, int count)
{
}

void Inventory::RemoveItem(Item* item, int count)
{
}

Item* Inventory::GetCurrentItem() const
{
	if (m_CurrIndex >= 0 && m_CurrIndex < m_MaxSize)
	{
		return m_Inventory[m_CurrIndex].first;
	}
	return nullptr;
}

void Inventory::SetCurrentIndex(int index)
{
	m_CurrIndex = index;
	m_UI->SetCurrentIndex(m_CurrIndex);
}

const std::pair<Item*, int>& Inventory::GetInventoryData(int index) const
{
	return m_Inventory[index];
}

