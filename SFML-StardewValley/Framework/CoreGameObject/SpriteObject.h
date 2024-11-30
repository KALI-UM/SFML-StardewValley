#pragma once
#include "GameObject.h"
class DSprite;
class SpriteObject :
    public GameObject
{
public:
    SpriteObject() {};
    SpriteObject(const std::string& texId);
    ~SpriteObject();

    bool Initialize() override;
    void Reset() override;

    void SetTexture(const std::string& filepath);
    void SetTexture(const sf::Texture* tex);
protected:
    std::string m_TextureId;
    DSprite* m_Sprite;
};

