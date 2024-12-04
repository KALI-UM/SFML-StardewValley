#include "pch.h"
#include "TileObjDataTable.h"
#include "TexCoordTable.h"

TileObjData TileObjDataTable::m_Empty = TileObjData();

TileObjDataTable::TileObjDataTable()
	:DataTable(DataId::TileObjData, "datatables/TileObjData.csv")
{
}

TileObjDataTable::~TileObjDataTable()
{
}

bool TileObjDataTable::Load()
{
	Release();
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(0, -1));
	for (int j = 0; j < doc.GetRowCount(); j++)
	{
		TileObjData curr;
		TOBJID id = doc.GetCell<std::string>("id", j);
		auto it = m_TileObjectDatas.find(curr.id);
		if (it != m_TileObjectDatas.end())
		{
			std::cout << "TileObjDataTable : " << curr.id << "is already exist." << std::endl;
			continue;
		}

		LoadTileObjFile(curr, doc.GetCell<std::string>("filepath", j));
		m_TileObjectDatas.insert({ id, curr });
	}

	return true;
}

void TileObjDataTable::Release()
{
	m_TileObjectDatas.clear();
}

const TileObjData& TileObjDataTable::GetTileObjectData(const TOBJID& id) const
{
	if (id == "")return m_Empty;

	auto it = m_TileObjectDatas.find(id);
	if (it == m_TileObjectDatas.end())
		return m_Empty;

	return it->second;
}

void TileObjDataTable::LoadTileObjFile(TileObjData& data, const std::string& filepath)
{
	TileObjRawData raw;
	LoadTileObjRawFile(raw, filepath);
	data.id = raw.id;
	data.originIndex = raw.originIndex;
	for (auto& yv : raw.tileTypeMap)
	{
		data.tileTypeMap.push_back(std::vector<TileObjData::UnitData>());
		for (auto& xv : yv)
		{
			TileObjData::UnitData ud;
			ud.texres = &TEXRESTABLE_MGR->GetTileTexRes(xv.texid);
			ud.type = Tile::StringToTileType(xv.type);
			ud.subtype = xv.subtype;
			data.tileTypeMap[data.tileTypeMap.size()-1].push_back(ud);
		}
	}
	data.uuSize = raw.uuSize;
}

void TileObjDataTable::LoadTileObjRawFile(TileObjRawData& data, const std::string& filepath)
{
	std::ifstream f(filepath);
	json tobjfile = json::parse(f);
	data = tobjfile.get<TileObjRawData>();
}
