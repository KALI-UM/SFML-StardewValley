#pragma once

class UIObject :
    public GameObject
{
    friend class SceneBase;
public:
    UIObject(const std::string& name="");
    ~UIObject();

    bool GetHasFocus()const { return GetIsActive() && m_HasFocus; }
protected:
    bool    m_HasFocus = false; 
    int     m_UIViewIndex = 0;
};

