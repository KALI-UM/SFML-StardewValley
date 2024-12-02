#pragma once
#include "Tile/Tile.h"

class DTile;
class TileObject :
	public GameObject
{
public:
	TileObject();
	~TileObject();

	bool Initialize() override;


	DTile* GetObjectTile(const CellIndex& tileIndex) { return m_TileSprite; };
	ColliderType GetColliderType(const CellIndex& currIndex) const;

	void OnColliderEnter(const CellIndex& currIndex);
	void OnColliderStay(const CellIndex& currIndex);
	void OnColliderExit(const CellIndex& currIndex);

	virtual void OnTriggerEnter(const CellIndex& localIndex) {};
	virtual void OnTriggerStay(const CellIndex& localIndex) {};
	virtual void OnTriggerExit(const CellIndex& localIndex) {};

protected:
	CellIndex									m_TileIndex;
	CellIndex									m_TLIndex;
	std::vector <std::vector<ColliderType>>		m_TLOffsets;

	DTile* m_TileSprite;
};

