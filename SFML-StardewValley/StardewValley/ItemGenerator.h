#pragma once
#include "Item/Item.h"

class Scene_InGame;
class InGamePlayer;
class ItemGenerator :
    public Singleton<ItemGenerator>
{

public:
    void Enter(Scene_InGame* scene);

    Item* GetItem(const ITEMID& itemId);
    const InGamePlayer* GetPlayer() const { return m_Player; }
    void DropItemToMap(const ITEMID& id, const sf::Vector2f& pos);
protected:
    Scene_InGame* m_CurrInGameScene;
    inline static const InGamePlayer* m_Player = nullptr;
};

#define ITEMGENERATOR (ItemGenerator::GetInstance())
