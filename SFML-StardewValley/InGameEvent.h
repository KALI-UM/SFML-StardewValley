#pragma once

class Scene_InGame;
class InGameEvent :
	public Singleton<InGameEvent>
{
	friend class Singleton<InGameEvent>;
protected:
	InGameEvent() = default;
	virtual ~InGameEvent() = default;

	InGameEvent(const InGameEvent& other) = delete;
	InGameEvent& operator=(const InGameEvent& other) = delete;

public:
	void Enter(Scene_InGame* scene);

	void EVENT(const std::string& subtype);

	void ENTER(const std::string& scene);
	void ENDDAY();

	inline static sf::Vector2i m_EnterPoint = { 3,10 };

protected:
	Scene_InGame* m_CurrInGameScene;
	static inline float			m_DelayTimer=-1;
};

#define INGAMEEVENT (InGameEvent::GetInstance())