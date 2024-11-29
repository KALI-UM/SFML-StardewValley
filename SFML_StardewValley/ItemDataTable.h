#pragma once
#include "DataTable.h"
#include "Item/Item.h"

class ItemDataTable :
    public DataTable
{
public:
    ItemDataTable();
    ~ItemDataTable();

    bool Load() override;
    void Release() override;

    const ItemDataRes& GetItemDataRes(const ITEMID& id) const;

protected:
    std::unordered_map<ITEMID, ItemDataRes> m_ItemData;
    static ItemDataRes m_Empty;

};

