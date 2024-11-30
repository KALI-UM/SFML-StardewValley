#include "pch.h"
#include "Item/Item.h"
#include "magic_enum.hpp"
#include "ItemDataTable.h"

Item::Item(const ITEMID& itemId, ItemType type)
	:m_ItemId(itemId), m_ItemType(type), m_StackType(SetStackTypeByItemType(type))
{
}

bool Item::Initialize()
{
	//������ ���̵�� ������ ���̺��� ������ �̸�, ������ texid�� �����͸� ���ͼ� �־��ش�.
	//�ϴ��� ���� �����͸� ����ִ�.
	LoadItemData();

	return false;
}

void Item::LoadItemData()
{
	const ItemDataRes& itemdata = ITEMDATATABLE_MGR->GetItemDataRes(m_ItemId);
	
	if (itemdata.name == "")
	{
		std::cout << "Item : " << m_ItemId << "is not Valid" << std::endl;
		return;
	}
	else
	{
		m_ItemName = itemdata.name;
		m_IconTexId = itemdata.iconTexId;
		for (auto& equiptexId : itemdata.equipTexId)
		{
			m_EquipTexIds.push_back(equiptexId);
		}
	}
}

std::string Item::ItemTypeToString(ItemType type)
{
	return std::string(magic_enum::enum_name(type).data());
}

ItemType Item::StringToItemType(const std::string& str)
{
	auto itemtype_str = magic_enum::enum_cast<ItemType>(str);
	if (itemtype_str.has_value())
	{
		return itemtype_str.value();
	}
	else
	{
		return ItemType::Other;
	}
}

StackType Item::SetStackTypeByItemType(ItemType type)
{
	if (type == ItemType::Tool)
		return StackType::NoneStackable;
	return StackType::Stackable;
}
