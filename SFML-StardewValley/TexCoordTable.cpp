#include "pch.h"
#include "TexCoordTable.h"

TexRes TexCoordTable::m_Empty = TexRes();


TexCoordTable::TexCoordTable()
	:DataTable(DataId::TexId, "datatables/AtlasSlicer.csv")
{
}

TexCoordTable::~TexCoordTable()
{
}

bool TexCoordTable::Load()
{
	Release();
	rapidcsv::Document doc(m_FilePath, rapidcsv::LabelParams(1, -1));
	for (int j = 0; j < doc.GetRowCount(); j++)
	{
		TexRes curr;
		curr.id = doc.GetCell<std::string>("id", j);
		auto it = m_TileTexCoord.find(curr.id);
		if (it != m_TileTexCoord.end())
		{
			std::cout << "TileTexCoordTable : " << curr.id << "is already exist." << std::endl;
			continue;
		}
		curr.filepath = m_TileFilePath + doc.GetCell<std::string>("filename", j);
		curr.uuSize = Get2Number(doc.GetCell<std::string>("unitxunit", j), "x").To<unsigned int>();
		curr.index = Get2Number(doc.GetCell<std::string>("index", j), ",");
		curr.texcoord.left = doc.GetCell<int>("left", j);
		curr.texcoord.top = doc.GetCell<int>("top", j);
		curr.texcoord.width = doc.GetCell<int>("width", j);
		curr.texcoord.height = doc.GetCell<int>("height", j);
		if (curr.uuSize != sf::Vector2u(1, 1))
			curr.children = std::vector<std::vector<TEXID>>(curr.uuSize.y, std::vector<std::string>(curr.uuSize.x));
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

void TexCoordTable::Release()
{
	m_TileTexCoord.clear();
}

const sf::IntRect& TexCoordTable::GetTexIntRect(const TEXID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty.texcoord;
	else
		return it->second.texcoord;
}

sf::FloatRect TexCoordTable::GetTexFloatRect(const TEXID& id) const
{
	return sf::FloatRect(GetTexIntRect(id));
}

const TexRes& TexCoordTable::GetTileTexRes(const TEXID& id) const
{
	auto it = m_TileTexCoord.find(id);
	if (it == m_TileTexCoord.end())
		return m_Empty;
	else
		return it->second;
}

sf::Vector2i TexCoordTable::Get2Number(const std::string& str, const std::string& separator)
{
	auto xIndex = str.find_first_of(separator);
	if (xIndex == std::string::npos)
		return sf::Vector2i(0,0);
	return sf::Vector2i(std::stoi(str.substr(0, xIndex)), std::stoi(str.substr(xIndex + separator.length(), str.length() - xIndex + separator.length())));
}
