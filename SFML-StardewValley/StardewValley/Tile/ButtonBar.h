#pragma once

enum class Action;
class D9SliceSprite;
class SubButtonBar;
class DTile;
class ButtonBar
	:public GameObject
{
public:
	ButtonBar(int viewIndex);
	~ButtonBar();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void SetButtonTex(const std::string& filepath, Action action, const sf::IntRect& rect);
	void SetButtonFunc(Action action, std::function<void()> func);
	bool GetHasFocus()const { return m_HasFocus; }

protected:
	std::string m_BarTexId;

	D9SliceSprite* m_Bar;
	std::vector<DTile*> m_Buttons;
	std::vector <std::function<void()>> m_ButtonFuncs;

	int m_ViewIndex = 0;
	int m_ButtonCount = 88;
	int m_CurrButtonIndex = 0;

private:
	bool			m_HasFocus = false;
};

