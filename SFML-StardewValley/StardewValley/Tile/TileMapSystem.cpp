#include "pch.h"
#include "TileMapSystem.h"
#include "TileModel.h"
#include "TileObjDataTable.h"

TileMapSystem::TileMapSystem(TileModel* model)
	:mcv_Model(model)
{
	//Tile::Initialize();
}

TileMapSystem::~TileMapSystem()
{
}

bool TileMapSystem::Initialize()
{
	//for (int layer = 0; layer < mcv_Model->m_LayerCnt; layer++)
	//{
	//	LoadTileViewRawFile((TileEditorLayer)layer);
	//}
	//LoadTileTypeFile();
	return true;
}

void TileMapSystem::Reset()
{
	m_CurrLayer = TileEditorLayer::Layer0;
}

void TileMapSystem::Update(float dt)
{
}

void TileMapSystem::LoadTileViewRawFile(TileEditorLayer layer)
{
	m_CurrLayer = layer;
	rapidcsv::Document doc("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv", rapidcsv::LabelParams(-1, -1));

	int cellxcnt = std::min((unsigned int)doc.GetColumnCount(), mcv_Model->m_CellCount.x);
	int cellycnt = std::min((unsigned int)doc.GetRowCount(), mcv_Model->m_CellCount.y);

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string strings = doc.GetCell<std::string>(i, j);
			BuildTilesById({ i,j }, strings);
		}
	}
}

void TileMapSystem::SaveTileViewRawFile(TileEditorLayer layer, const std::string& filename)
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileInfo& currInfo = mcv_Model->GetTileInfo((int)layer, { i,j });
			std::string strings = currInfo.id;
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/" + filename + "tex.csv");
}

void TileMapSystem::SaveTileViewRawFile(TileEditorLayer layer)
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			const TileInfo& currInfo = mcv_Model->GetTileInfo((int)layer, { i,j });
			std::string strings = currInfo.id;
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileInfo_layer" + std::to_string((int)layer) + ".csv");
}

void TileMapSystem::LoadTileTypeFile()
{
	rapidcsv::Document doc("datatables/tileTypeInfo_layer" + std::to_string((int)0) + ".csv", rapidcsv::LabelParams(-1, -1));

	int cellxcnt = std::min((unsigned int)doc.GetColumnCount(), mcv_Model->m_CellCount.x);
	int cellycnt = std::min((unsigned int)doc.GetRowCount(), mcv_Model->m_CellCount.y);

	m_TileTypeInfos = std::vector<std::vector<TileType>>(mcv_Model->m_CellCount.y, std::vector<TileType>(mcv_Model->m_CellCount.x, TileType::None));

	for (int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			std::string strings = doc.GetCell<std::string>(i, j);
			m_TileTypeInfos[j][i] = Tile::StringToTileType(strings);
		}
	}
}

void TileMapSystem::SaveTileTypeFile()
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			std::string strings = Tile::TileTypeToString(m_TileTypeInfos[j][i]);
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/tileTypeInfo_layer" + std::to_string((int)0) + ".csv");
}

void TileMapSystem::SaveTileTypeFile(const std::string& filename)
{
	rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1));
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			std::string strings = Tile::TileTypeToString(m_TileTypeInfos[j][i]);
			doc.SetCell(i, j, strings);
		}
	}
	doc.Save("datatables/" + filename + "type.csv");

}

void TileMapSystem::SaveAsTileObjData(const std::string& tileObjId, const std::string& texfilepath, const std::string& typefilepath)
{
	rapidcsv::Document texdoc(texfilepath, rapidcsv::LabelParams(-1, -1));
	rapidcsv::Document typedoc(typefilepath, rapidcsv::LabelParams(-1, -1));
	int cellxcnt = std::min(texdoc.GetColumnCount(), typedoc.GetColumnCount());
	int cellycnt = std::min(texdoc.GetRowCount(), typedoc.GetColumnCount());

	TileObjRawData tobj;
	tobj.id = tileObjId;
	tobj.originIndex = { 0,0 };
	tobj.uuSize = { (unsigned int)cellxcnt, (unsigned int)cellycnt };
	tobj.tileTypeMap = std::vector<std::vector<TileObjRawData::UnitData>>(cellycnt, std::vector<TileObjRawData::UnitData>(cellxcnt));
	for(int j = 0; j < cellycnt; j++)
	{
		for (int i = 0; i < cellxcnt; i++)
		{
			TileObjRawData::UnitData& currunit = tobj.tileTypeMap[j][i];
			currunit.texid = texdoc.GetCell<std::string>(i, j);
			currunit.type = typedoc.GetCell<std::string>(i, j);	
		}
	}

	json tobjfile = tobj;
	std::ofstream f("datatables/TileObj/"+ tileObjId+".json");
	f << tobjfile.dump(4) << std::endl;
	f.close();
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const UNITxUNIT& uu, const std::list<TEXID>& ids)
{
	std::vector<std::vector<TEXID>> id_uu = std::vector<std::vector<TEXID>>(uu.y, std::vector<TEXID>(uu.x));
	auto idit = ids.begin();
	for (int uuy = 0; uuy < (int)uu.y; uuy++)
	{
		for (int uux = 0; uux < (int)uu.x; uux++)
		{
			id_uu[uuy][uux] = *idit;
			idit++;
		}
	}

	for (auto& currIndex : tiles)
	{
		for (int uuy = 0; uuy < (int)uu.y; uuy++)
		{
			for (int uux = 0; uux < (int)uu.x; uux++)
			{
				sf::Vector2i uuoffset(uux, uuy);

				if (mcv_Model->IsValidTileIndex(currIndex + uuoffset))
					mcv_Model->SetTile((int)m_CurrLayer, currIndex + uuoffset, id_uu[uuy][uux]);
			}
		}
	}
}

void TileMapSystem::BuildTilesById(const std::list<CellIndex> tiles, const TEXID& id)
{
	mcv_Model->SetTiles((int)m_CurrLayer, tiles, id);
}

void TileMapSystem::BuildTilesById(const CellIndex& tile, const TEXID& id)
{
	mcv_Model->SetTile((int)m_CurrLayer, tile, id);
}

void TileMapSystem::SetTilesType(const std::list<CellIndex>& tiles, TileType type)
{
	for (auto& currIndex : tiles)
	{
		SetTileType(currIndex, type);
	}
}

void TileMapSystem::SetTileType(const CellIndex& tileIndex, TileType type)
{
	m_TileTypeInfos[tileIndex.y][tileIndex.x] = type;
}

void TileMapSystem::SetTileTypeMode(TileType type)
{
	for (int j = 0; j < mcv_Model->m_CellCount.y; j++)
	{
		for (int i = 0; i < mcv_Model->m_CellCount.x; i++)
		{
			auto& currTypeInfo = m_TileTypeInfos[j][i];

			if (type == TileType::Wall && currTypeInfo == TileType::Wall)
				RequestColorizeTile(sf::Color(255, 100, 100), { i,j });
			else if (type == TileType::Water && currTypeInfo == TileType::Water)
				RequestColorizeTile(sf::Color(100, 100, 255), { i,j });
			else if (type == TileType::Ground && currTypeInfo == TileType::Ground)
				RequestColorizeTile(sf::Color(125, 125, 125), { i,j });
			else if (type == TileType::Soil && currTypeInfo == TileType::Soil)
				RequestColorizeTile(sf::Color(125, 125, 125), { i,j });
		}
	}
}



void TileMapSystem::RequestColorizeTile(const sf::Color& color, const CellIndex& tileIndex)
{
#ifdef DEBUG
	if (m_WhenNeedsToColorizeTileFunc)
#endif // DEBUG
		m_WhenNeedsToColorizeTileFunc(color, tileIndex);
#ifdef DEBUG
	else
		std::cout << "TileModel::RequestColorizeTile-Fail" << std::endl;
#endif // DEBUG
}

