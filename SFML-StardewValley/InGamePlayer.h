#pragma once
#include "Tile/Tile.h"


enum class PlayerStatus
{

};
class TileObjectSystem;
class Inventory;
class InGamePlayer :
    public GameObject
{
public:
    InGamePlayer(const std::string& name);
    ~InGamePlayer();

	bool Initialize();
	void Reset();
	void Update(float dt);

	bool IsPlayerNearbyTile(const CellIndex& index);
	void SetTileSystem(TileObjectSystem* const sys) { m_TileSystem = sys; };
	void SetInventory(Inventory* inventory);
	bool GetItem(const std::string& itemId, int count) const;
	const sf::FloatRect& GetGetItemArea() const { return m_GetItemArea->GetGlobalBounds(); }
protected:
	DSprite* m_PlayerBody;
	Inventory* m_Inventory;
	DRectangle* m_GetItemArea;

	sf::Vector2f m_Direction;
	float m_MoveSpeed = 100.f;
	int m_MaxStamina = 20;
	int m_Satmina = m_MaxStamina;

	CellIndex m_TileIndex;
	const TileObjectSystem* m_TileSystem;
};

 