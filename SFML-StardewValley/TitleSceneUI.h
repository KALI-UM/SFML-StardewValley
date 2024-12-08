#pragma once

class TitleSceneUI :
    public UIObject
{
public:
    TitleSceneUI();
    ~TitleSceneUI();

    bool Initialize() override;
    void Reset()override;
    void Update(float dt);
    void Release()override;

protected:
    DSprite* m_Logo;
    DSprite* m_Buttons[4];

    void Button(int index);

    int m_PrevButton = -1;
    int m_NowButton;
    bool    m_NeedWait = false;
    float   m_Timer = 0.0f;
};

