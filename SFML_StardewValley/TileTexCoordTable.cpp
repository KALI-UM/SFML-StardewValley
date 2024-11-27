#include "pch.h"
#include "TileTexCoordTable.h"

TileTexRes TileTexCoordTable::m_Empty = TileTexRes();


TileTexCoordTable::TileTexCoordTable()
	:DataTable(DataId::TileTexRes, "datatables/AtlasSlicer.csv")
{
}

TileTexCoordTable::~TileTexCoordTable()
{
}

bool TileTexCoordTable::Load()
{
	Release();
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(0, -1));
	for (int j = 0; j < doc.GetRowCount(); j++)
	{
		TileTexRes curr;
		curr.id = doc.GetCell<std::string>("id", j);
		auto it = m_TileTexCoord.find(curr.id);
		if (it != m_TileTexCoord.end())
		{
			std::cout << "TileTexCoordTable : " << curr.id << "is already exist." << std::endl;
			continue;
		}
		curr.filepath = m_TileFilePath + doc.GetCell<std::string>("filename", j);
		curr.texcoord.left = doc.GetCell<float>("x", j);
		curr.texcoord.top = doc.GetCell<float>("y", j);
		curr.texcoord.width = doc.GetCell<float>("width", j);
		curr.texcoord.height = doc.GetCell<float>("height", j);
	}

	for (int j = 0; j < doc.GetRowCount(); j++)
	{
		std::string currId = doc.GetCell<std::string>("id", j);
		auto it = m_TileTexCoord.find(currId);
		if (it == m_TileTexCoord.end())
		{
			continue;
		}
		auto underbar = currId.find('_');
		if (underbar == std::string::npos)
			continue;

		m_TileTexCoord.find(currId.substr(0, underbar))->second.children.push_back(currId);
	}
	return true;
}

void TileTexCoordTable::Release()
{
	m_TileTexCoord.clear();
}

const sf::IntRect& TileTexCoordTable::GetTexIntRect(const ID& id) const
{
	return sf::IntRect(GetTexFloatRect(id));
}

const sf::FloatRect& TileTexCoordTable::GetTexFloatRect(const ID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty.texcoord;
	else
		return it->second.texcoord;
}

const TileTexRes& TileTexCoordTable::GetTileTexRes(const ID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty;
	else
		return it->second;
}
