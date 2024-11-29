#include "pch.h"
#include "ItemDataTable.h"

ItemDataRes ItemDataTable::m_Empty = ItemDataRes();

ItemDataTable::ItemDataTable()
	:DataTable(DataId::ItemData, "datatables/ItemData.csv")
{
}

ItemDataTable::~ItemDataTable()
{
}

bool ItemDataTable::Load()
{
	Release();
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(0, -1));
	for (int j = 0; j < doc.GetRowCount(); j++)
	{
		ItemDataRes curr;
		curr.id = doc.GetCell<std::string>("id", j);
		auto it = m_ItemData.find(curr.id);
		if (it != m_ItemData.end())
		{
			std::cout << "ItemDataTable : " << curr.id << "is already exist." << std::endl;
			continue;
		}
		curr.name =doc.GetCell<std::string>("name", j);
		curr.type = Item::StringToItemType(doc.GetCell<std::string>("type", j));
		curr.iconTexId = doc.GetCell<std::string>("iconId", j);
		curr.equipTexId = doc.GetCell<std::string>("equipId", j);	

		m_ItemData.insert({ curr.id, curr });
	}

	return true;
}

void ItemDataTable::Release()
{
	m_ItemData.clear();
}

const ItemDataRes& ItemDataTable::GetItemDataRes(const ITEMID& id) const
{
	auto it = m_ItemData.find(id);
	if (it == m_ItemData.end())
		return m_Empty;
	else
		return it->second;
}
