#pragma once

class GameInfoUI :
    public UIObject
{
public:
    GameInfoUI();
    ~GameInfoUI();

    bool Initialize() override;
    void Reset()override;
    void Update(float dt);
    void Release()override;

protected:
    DSprite* m_ClockBody;
    DSprite* m_ClockPointer;
};

