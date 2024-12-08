#pragma once

enum class InfoTab
{
    Inventory,
    CharactorInfo,
    Max,
};


class D9SliceSprite;
class PlayerInfoUI :
    public UIObject
{
public:
    PlayerInfoUI();
    ~PlayerInfoUI();

    bool Initialize() override;
    void Reset()override;
    void Update(float dt)override;
    void Release()override;

protected:
    D9SliceSprite* m_TabSprite;

};

