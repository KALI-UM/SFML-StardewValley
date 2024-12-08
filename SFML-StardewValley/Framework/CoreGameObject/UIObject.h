#pragma once

class UIObject :
    public GameObject
{
    friend class SceneBase;
public:
    UIObject(const std::string& name="");
    ~UIObject();

    bool GetHasFocus()const { return GetIsActive() && m_HasFocus; }
    void SetAspectRatioPosition(const sf::Vector2f& aspect, const sf::Vector2f& offset);
    void OnWindowResize();
protected:
    bool    m_HasFocus = false; 
    int     m_UIViewIndex = 0;
    sf::Vector2f    m_AspectPositionRatio;
    sf::Vector2f    m_AspectPositionOffset;
};

