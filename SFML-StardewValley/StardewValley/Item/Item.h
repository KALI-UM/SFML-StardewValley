#pragma once

typedef std::string ITEMID;
typedef std::string NAME;
typedef std::string TEXID;

enum class ItemType
{
	Tool,
	Foraging,   //ä��
	Crop,		//�۹�
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

	virtual void Use(Player* const player) = 0;				//use�Լ��� �ڽ� �����۵��� �������ؼ� ����� ���Դϴ�.

protected:
	NAME		m_ItemName = "";
	int			m_Count = 1;

	TEXID		m_IconTexId;
	std::vector<TEXID>		m_EquipTexIds;				//���⺰ �����Ұ����� ����...

public:
	inline static const int m_StackMax = 64;

	static std::string ItemTypeToString(ItemType type);
	static ItemType StringToItemType(const std::string& str);
private:
	void LoadItemData();

	static StackType SetStackTypeByItemType(ItemType type);
};

