#include "pch.h"
#include "TileObject_Tree.h"
#include "DTile.h"
#include "TexCoordTable.h"

bool TileObject_Tree::Initialize()
{
	TileObject::Initialize();

	return true;
}

void TileObject_Tree::Reset()
{
	const TexRes& treeBodyTex = TEXRESTABLE_MGR->GetTileTexRes("Trees.png#Tree01");
	LOT lot = treeBodyTex.lotSize;

	m_TileSprite->SetTexture(treeBodyTex.filepath);
	m_TileSprite->SetTextureRect(treeBodyTex.texcoord, treeBodyTex.lotSize);
	const TexRes& treeStumpTex = TEXRESTABLE_MGR->GetTileTexRes("Trees.png#Tree01Stump_X0Y1");
	m_TileSprite->SetVerticesTexCoordByIntRect(16, treeStumpTex.texcoord);

	m_TLOffsets = std::vector<std::vector<ColliderType>>(lot.y, std::vector<ColliderType>(lot.x));
	
	for (int y = 0; y < (int)lot.y; y++)
	{
		for (int x = 0; x < (int)lot.x; x++)
		{
			m_TLOffsets[y][x] = ColliderType::None;
		}
	}
	m_TLOffsets[5][1] = ColliderType::Block;

}
