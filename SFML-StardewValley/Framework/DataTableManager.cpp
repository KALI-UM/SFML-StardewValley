#include "pch.h"
#include "DataTableManager.h"
#include "TexCoordTable.h"
#include "ItemDataTable.h"
#include "TileObjDataTable.h"

DataTableManager::~DataTableManager()
{
	Release();
}

bool DataTableManager::Initialize()
{
	Release();
	bool result = true;
	m_DataTables.insert({ DataTable::DataId::TexId, new TexCoordTable() });
	m_DataTables.insert({ DataTable::DataId::ItemData, new ItemDataTable() });
	m_DataTables.insert({ DataTable::DataId::TileObjData, new TileObjDataTable() });

	for (auto t : m_DataTables)
	{
		result&=t.second->Load();
	}
	return result;
}

void DataTableManager::Release()
{
	for (auto t : m_DataTables)
	{
		t.second->Release();
		delete t.second;
	}
	m_DataTables.clear();
}