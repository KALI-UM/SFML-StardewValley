#pragma once

typedef std::string UIID;

class UIObject;
class UIManager :
	public Singleton<UIManager>
{
	friend class Singleton<UIManager>;
protected:
	UIManager() = default;
	virtual ~UIManager() = default;

	UIManager(const UIManager& other) = delete;
	UIManager& operator=(const UIManager& other) = delete;

public:

	void Enter();
	void Update(float dt);
	void Exit();

	void RegisterUI(UIObject* ui);
	sf::Vector2f GetMouseUIViewPos() const;
	bool GetUIHasFocus() const { return m_UIHasFocus; }

protected:

	int     m_UIViewIndex = 0;
	bool	m_UIHasFocus = false;

	std::list<UIObject*> m_UIGameObjects;
};

