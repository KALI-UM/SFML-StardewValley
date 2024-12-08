#pragma once

typedef std::string ITEMID;
class Item;
class InventoryUI;
class Inventory :
	public GameObject
{
public:
	Inventory();
	~Inventory();

	bool Initialize() override;

	void SetInventoryUI(InventoryUI* ui) { m_UI = ui; };

	bool PushItem(const ITEMID& item, int count = 1);
	void RemoveItem(const ITEMID& item, int count = 1);
	void RemoveItem(Item* item, int count = 1);

	int		GetCurrentIndex()const { return m_CurrIndex; }
	Item*	GetCurrentItem() const;
	void	SetCurrentIndex(int index);

	const std::pair<Item*, int>& GetInventoryData(int index) const;
	int GetInventoryMaxSize() const { return m_MaxSize; }
protected:
	int									m_CurrIndex;
	std::vector<std::pair<Item*, int>>	m_Inventory;
	int									m_MaxSize = 10;
	InventoryUI* m_UI;
};

