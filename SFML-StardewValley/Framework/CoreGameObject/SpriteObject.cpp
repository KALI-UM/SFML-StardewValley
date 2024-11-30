#include "pch.h"
#include "SpriteObject.h"
#include "DSprite.h"

SpriteObject::SpriteObject(const std::string& texId)
    :m_TextureId(texId)
{
}

SpriteObject::~SpriteObject()
{
}

bool SpriteObject::Initialize()
{
    m_Sprite = new DSprite();
    SetDrawableObj(m_Sprite);
    return false;
}

void SpriteObject::Reset()
{
    if (m_TextureId != "")
        SetTexture(m_TextureId);
}

void SpriteObject::SetTexture(const std::string& filepath)
{
    m_Sprite->SetTexture(filepath, true);
}

void SpriteObject::SetTexture(const sf::Texture* tex)
{
    m_Sprite->SetTexture(tex, true);
}
