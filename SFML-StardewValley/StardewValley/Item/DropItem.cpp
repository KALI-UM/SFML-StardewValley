#include "pch.h"
#include "DropItem.h"
#include "ItemDataTable.h"
#include "TexCoordTable.h"
#include "InGamePlayer.h"
#include "ItemGenerator.h"


bool DropItem::IsNeedToReturn()
{
	return m_TaskDone;
}

void DropItem::InitForObjectPool()
{
	this->INITIALIZE();
}

void DropItem::ResetForObjectPool()
{
	this->RESET();
}

bool DropItem::Initialize()
{
	m_ItemIcon = new DSprite();
	SetDrawableObj(m_ItemIcon);
	m_Shadow = new DCircle();
	SetDrawableObj(m_Shadow);
	return true;
}

void DropItem::Reset()
{
	m_Timer = -1.0f;
	m_Shadow->SetPointCount(8);
	m_Shadow->SetFillColor(sf::Color(80, 80, 80, 125));
	m_Shadow->SetRadius(4);
	m_Shadow->SetOutlineThickness(0);
	m_Shadow->SetOrigin(OriginType::MC);
	m_Shadow->setLocalScale({ 1.0f, 0.5f });
	m_Shadow->setLocalPosition({ 0, 0.4f });
	m_ItemIcon->setLocalPosition({ 0,0 });
}

void DropItem::Update(float dt)
{
	if (m_Timer != -1.0f)
	{
		m_Timer += dt;
		sf::Vector2f pos = Utils::Lerp(m_StartPosition, m_TargetPosition, m_Timer / 1.0f);
		m_ItemIcon->setLocalPosition({ 0, -8.f * sinf(m_Timer * 3.14f / 1.0f) });
		setPosition(pos);

		if (m_Timer > 1.0f)
			m_Timer = -1.0f;
	}

	if (m_Timer > 0.25f || m_Timer == -1.0f)
	{
		if (m_ItemIcon->GetGlobalBounds().intersects(m_Player->GetGetItemArea()))
		{
			if (m_Player->GetItem(m_ItemId, 1))
			{
				SOUND_MGR->PlaySfx("sounds/Interface/pickUpItem.wav");
				m_TaskDone = true;
				SetIsActive(false);
			}
		}
	}
}

void DropItem::Load(const ITEMID& id)
{
	m_ItemId = id;
	const ItemDataRes& itemdata = ITEMDATATABLE_MGR->GetItemDataRes(m_ItemId);
	const TexRes& texres = TEXRESTABLE_MGR->GetTileTexRes(itemdata.iconTexId);

	m_ItemIcon->SetTexture(texres.filepath);
	m_ItemIcon->SetTextureRect(texres.texcoord);
	m_ItemIcon->SetOrigin(OriginType::BC);
	m_Timer = -1.0f;
	m_TaskDone = false;

	m_Player = ITEMGENERATOR->GetPlayer();
}

void DropItem::FloatingEffect(const sf::Vector2f& pos)
{
	//드롭아이템 근방 16이내에 포지션으로 드롭아이템이 뽀잉하고 움직이는 느낌을 주려는 의도
	m_StartPosition = pos;
	m_TargetPosition = pos + Utils::RandomInUnitCircle() * 16.0f;
	m_Timer = 0;
}
