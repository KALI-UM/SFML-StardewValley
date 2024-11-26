#include "pch.h"
#include "DataTableManager.h"

DataTableManager::~DataTableManager()
{
	Release();
}

bool DataTableManager::Initialize()
{
	Release();

	bool result = true;

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