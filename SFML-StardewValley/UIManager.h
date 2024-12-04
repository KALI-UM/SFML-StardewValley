#pragma once

typedef std::string UIID;

class UI;
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


protected:
	std::unordered_map<UIID, UI*> m_UIGameObjects;
};

