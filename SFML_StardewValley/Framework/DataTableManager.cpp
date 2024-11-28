#include "pch.h"
#include "DataTableManager.h"
#include "TileTexCoordTable.h"

DataTableManager::~DataTableManager()
{
	Release();
}

bool DataTableManager::Initialize()
{
	Release();
	bool result = true;
	m_DataTables.insert({ DataTable::DataId::TileTexRes, new TileTexCoordTable() });

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