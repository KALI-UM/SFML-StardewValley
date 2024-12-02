#pragma once

typedef std::string ITEMID;
typedef std::string NAME;
typedef std::string TEXID;

enum class ItemType
{
	Tool,
	Foraging,   //채집
	Crop,		//작물
	Other,
};

struct ItemDataRes
{
	ITEMID id;
	NAME name;
	ItemType type;
	TEXID iconTexId;
	std::vector<TEXID> equipTexId;
};

enum class StackType
{
	Stackable,
	NoneStackable,
};

class Player;
class Item :
	public GameObject
{
protected:
	Item(const ITEMID& itemId, ItemType type);
public:
	~Item() {};

	const ITEMID	m_ItemId;
	const ItemType	m_ItemType;
	const StackType	m_StackType;

public:
	bool Initialize();

	virtual void Use(Player* const player) = 0;				//use함수를 자식 아이템들이 재정의해서 사용할 것입니다.

protected:
	NAME		m_ItemName = "";
	int			m_Count = 1;

	TEXID		m_IconTexId;
	std::vector<TEXID>		m_EquipTexIds;				//방향별 어찌할건지는 추후...

public:
	inline static const int m_StackMax = 64;

	static std::string ItemTypeToString(ItemType type);
	static ItemType StringToItemType(const std::string& str);
private:
	void LoadItemData();

	static StackType SetStackTypeByItemType(ItemType type);
};

