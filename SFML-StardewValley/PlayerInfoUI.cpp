#include "pch.h"
#include "PlayerInfoUI.h"
#include "D9SliceSprite.h"
#include "TexCoordTable.h"


bool PlayerInfoUI::Initialize()
{
    m_TabSprite = new D9SliceSprite();
    m_TabSprite->SetPriorityType(DrawPriorityType::Custom, 1);
    SetDrawableObj(m_TabSprite);
    return false;
}
