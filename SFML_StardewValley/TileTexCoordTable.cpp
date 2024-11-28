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
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(1, -1));
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
		curr.lotSize = Get2Number(doc.GetCell<std::string>("lot", j), "x").To<unsigned int>();
		curr.index = Get2Number(doc.GetCell<std::string>("index", j), ",");
		curr.texcoord.left = doc.GetCell<float>("left", j);
		curr.texcoord.top = doc.GetCell<float>("top", j);
		curr.texcoord.width = doc.GetCell<float>("width", j);
		curr.texcoord.height = doc.GetCell<float>("height", j);
		if (curr.lotSize != sf::Vector2u(1, 1))
			curr.children = std::vector<std::vector<ID>>(curr.lotSize.y, std::vector<std::string>(curr.lotSize.x));
		m_TileTexCoord.insert({ curr.id , curr });
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

		auto parent = m_TileTexCoord.find(currId.substr(0, underbar));
		parent->second.children[it->second.index.y][it->second.index.x] = currId;
	}
	return true;
}

void TileTexCoordTable::Release()
{
	m_TileTexCoord.clear();
}

const sf::IntRect& TileTexCoordTable::GetTexIntRect(const ID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty.texcoord;
	else
		return it->second.texcoord;
}

const sf::FloatRect& TileTexCoordTable::GetTexFloatRect(const ID& id) const
{
	return sf::FloatRect(GetTexIntRect(id));
}

const TileTexRes& TileTexCoordTable::GetTileTexRes(const ID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty;
	else
		return it->second;
}

sf::Vector2i TileTexCoordTable::Get2Number(const std::string& str, const std::string& separator)
{
	auto xIndex = str.find_first_of(separator);
	if (xIndex == std::string::npos)
		return sf::Vector2i(0,0);
	return sf::Vector2i(std::stoi(str.substr(0, xIndex)), std::stoi(str.substr(xIndex + separator.length(), str.length() - xIndex + separator.length())));
}
