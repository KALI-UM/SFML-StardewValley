#include "pch.h"
#include "DataTableManager.h"
#include "TexCoordTable.h"
#include "ItemDataTable.h"

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