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
    return true;
}

void DropItem::Reset()
{
    m_Timer = -1.0f;
}

void DropItem::Update(float dt)
{
    if (m_Timer != -1.0f)
    {
        m_Timer += dt;
        sf::Vector2f pos = Utils::Lerp(m_StartPosition, m_TargetPosition, m_Timer / 3.0f);
        setPosition(pos+sf::Vector2f(0, sinf(m_Timer / 3.0f)));

        if (m_Timer > -3.0f)
            m_Timer = -1.0f;
    }

    if (m_ItemIcon->GetGlobalBounds().intersects(m_Player->GetGetItemArea()))
    {
        if (m_Player->GetItem(m_ItemId, 1))
        {
            m_TaskDone = true;
            SetIsActive(false);
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
    m_StartPosition = pos;
    m_TargetPosition = pos + Utils::RandomInUnitCircle() * 16.0f;
    m_Timer = 0;
}
