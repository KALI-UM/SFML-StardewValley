#include "pch.h"
#include "InventoryUI.h"
#include "D9SliceSprite.h"
#include "TexCoordTable.h"

InventoryUI::InventoryUI()
{
}

InventoryUI::~InventoryUI()
{
}

bool InventoryUI::Initialize()
{
	m_Frame = new D9SliceSprite();
	SetDrawableObj(m_Frame);
	m_Frame->SetPriorityType(DrawPriorityType::Custom, 10);
	m_Frame->SetDebugDraw(false);

	m_Background = new DRectangle();
	SetDrawableObj(m_Background);
	m_Background->SetPriorityType(DrawPriorityType::Custom, 0);
	m_Background->SetDebugDraw(false);

	for (int i = 0; i < 10; i++)
	{
		m_Block[i] = new DRectangle();
		SetDrawableObj(m_Block[i]);
		m_Block[i]->SetPriorityType(DrawPriorityType::Custom, 1);
		m_Block[i]->SetDebugDraw(false);
	}

	for (int i = 0; i < 10; i++)
	{
		m_Item[i] = new DRectangle();
		SetDrawableObj(m_Item[i]);
		m_Item[i]->SetPriorityType(DrawPriorityType::Custom, 2);
		m_Item[i]->SetDebugDraw(false);
	}

	for (int i = 0; i < 10; i++)
	{
		m_Count[i] = new DText();
		SetDrawableObj(m_Count[i]);
		m_Count[i]->SetPriorityType(DrawPriorityType::Custom, 3);
		m_Count[i]->SetDebugDraw(false);
	}

	return true;
}

void InventoryUI::Reset()
{
	m_Frame->SetTexture("ui/Menu Elements.png");
	m_Frame->SetCornerSize(16);
	m_Frame->SetSize({ 16 * 11.2f, 16 * 2.2f });
	m_Frame->SetAtlas(sf::FloatRect(0, 0, 16, 16), sf::FloatRect(32, 0, 16, 16), sf::FloatRect(48, 0, 16, 16),
		sf::FloatRect(0, 32, 16, 16), sf::FloatRect(0, 0, 0, 0), sf::FloatRect(48, 32, 16, 16),
		sf::FloatRect(0, 48, 16, 16), sf::FloatRect(32, 48, 16, 16), sf::FloatRect(48, 48, 16, 16));
	m_Frame->SetOrigin(OriginType::BC);


	m_Background->SetTexture("ui/Menu Elements.png");
	m_Background->SetTextureRect(sf::IntRect(16, 32, 16, 16));
	m_Background->SetSize({ 16 * 10.2f, 16 * 1.2 });
	m_Background->setLocalPosition({ 0, -8.2f });
	m_Background->SetOrigin(OriginType::BC);

	for (int i = 0; i < 10; i++)
	{
		m_Block[i]->SetTexture("ui/Menu Elements.png");
		m_Block[i]->SetTextureRect(sf::IntRect(32, 32, 16, 16));
		m_Block[i]->SetSize({ 16 , 16 });
		m_Block[i]->setLocalPosition({ (i - 4.5f) * 16.0f,-9.2f });
		m_Block[i]->SetOutlineThickness(-1);
		m_Block[i]->SetOutlineColor(ColorPalette::Transparent);
		m_Block[i]->SetOrigin(OriginType::BC);
	}

	for (int i = 0; i < 10; i++)
	{
		m_Item[i]->SetSize({ 15 , 15 });
		m_Item[i]->SetFillColor(ColorPalette::Transparent);
		m_Item[i]->setLocalPosition({ (i - 4.5f) * 16.0f,-9.2f });
		m_Item[i]->SetOrigin(OriginType::BC);
	}

	for (int i = 0; i < 10; i++)
	{
		m_Count[i]->SetFont("fonts/DOSGothic.ttf");
		m_Count[i]->SetCharacterSize(15);
		m_Count[i]->setLocalPosition({ (i - 4.5f) * 16.0f+7.0f,-10.0f });
		m_Count[i]->setLocalScale({ 0.25f, 0.25f });
		m_Count[i]->SetColor(ColorPalette::White);
		m_Count[i]->SetOutlineColor(ColorPalette::Black);
		m_Count[i]->SetOutlineThickness(3);
	}

	setScale({ 4, 4 });

	SetCurrentIndex(m_CurrentIndex);
	SetAspectRatioPosition(sf::Vector2f(0.5f, 1.0f), sf::Vector2f(0, -10));


	SetItemIcon(0, "Tools.png#AxeIcon", 1);
}

void InventoryUI::Release()
{
}

void InventoryUI::SetItemIcon(int index, const std::string& icontexId, int count)
{
	auto& texres = TEXRESTABLE_MGR->GetTileTexRes(icontexId);
	m_Item[index]->SetTexture(texres.filepath);
	m_Item[index]->SetTextureRect(texres.texcoord);
	m_Count[index]->SetString(std::to_string(count));
	m_Count[index]->SetOrigin(OriginType::BR);
}

void InventoryUI::SetCurrentIndex(int index)
{
	m_Block[m_CurrentIndex]->SetOutlineColor(ColorPalette::Transparent);
	m_CurrentIndex = index;
	if (m_CurrentIndex >= 0 && m_CurrentIndex < 10)
		m_Block[m_CurrentIndex]->SetOutlineColor(ColorPalette::Red);
}
