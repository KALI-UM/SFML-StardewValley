#pragma once

class D9SliceSprite;
class EventUI :
    public UIObject
{
public:
    EventUI();
    ~EventUI();

    bool Initialize() override;
    void Reset()override;
    void Update(float dt);
    void Release()override;

    void SetEventString(const std::wstring& str);
    void SetOKButtonFunc(std::function<void()> func) { m_OKFunc = func; }
    void SetNOButtonFunc(std::function<void()> func) { m_NOFunc = func; }
protected:
    D9SliceSprite*  m_Frame;
    DRectangle*     m_Background;
    DText*          m_EventText;
    DText*          m_OK;
    DText*          m_NO;

    std::function<void()> m_OKFunc;
    std::function<void()> m_NOFunc;
};

