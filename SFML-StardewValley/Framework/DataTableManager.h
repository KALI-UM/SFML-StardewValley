#pragma once

class DataTableManager :
    public Singleton<DataTableManager>
{
    friend class Singleton<DataTableManager>;

protected:
	DataTableManager() = default;
	virtual ~DataTableManager();

	DataTableManager(const DataTableManager& other) = delete;
	DataTableManager& operator=(const DataTableManager& other) = delete;

public:
	std::unordered_map<DataTable::DataId, DataTable*> m_DataTables;

public:
	bool Initialize();
	void Release();

	template<typename T>
	T* Get(DataTable::DataId id);
};

template<typename T>
inline T* DataTableManager::Get(DataTable::DataId id)
{
	auto find = m_DataTables.find(id);
	if (find == m_DataTables.end())
		return nullptr;
	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MGR (DataTableManager::GetInstance())
#define TEXRESTABLE_MGR (DATATABLE_MGR->Get<TexCoordTable>(DataTable::DataId::TexId))
#define ITEMDATATABLE_MGR (DATATABLE_MGR->Get<ItemDataTable>(DataTable::DataId::ItemData))