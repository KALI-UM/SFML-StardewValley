#pragma once

enum class Action;
class D9SliceSprite;
class SubButtonBar
	:public GameObject
{
public:
	SubButtonBar(int bttCnt, int viewIndex);
	~SubButtonBar();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void ShowSubBar(bool value);

	void SetButtonFunc(int bttIndex, std::function<void()> func) { m_ButtonFunc[bttIndex] = func; };
	void SetButtonText(int bttIndex, const std::wstring bttText);
	void SetButtonText(int bttIndex, const std::string bttText);
protected:
	std::string m_BarTexId;
	std::string m_FontId;

	D9SliceSprite* m_Bar;
	std::vector<DText*> m_Buttons;
	std::vector<std::function<void()>> m_ButtonFunc;
	int m_ViewIndex = 0;
	int m_ButtonCount = 0;
	int m_CurrButtonIndex = 0;
	float m_MaxSize=0;

	bool m_Delay = true;
};