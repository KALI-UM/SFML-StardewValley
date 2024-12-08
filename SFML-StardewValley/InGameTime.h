#pragma once

struct InGameDate
{
	int month = 3;
	int day = 1;
	int hour = 0;
	int min = 0;
};

struct InGameTimeConfig
{
	float realTimePerMinute = 0.02f;

	int hourPerDay = 20;

	int wakeUpTimeHour = 6;
	int sunSetTimeHour = 20;
};

enum class InGameTimeMode
{
	Play,
	Stop,
};

class Scene_InGame;
class InGameTime :
	public Singleton<InGameTime>
{
	friend class Singleton<InGameTime>;
protected:
	InGameTime() = default;
	virtual ~InGameTime() = default;

	InGameTime(const InGameTime& other) = delete;
	InGameTime& operator=(const InGameTime& other) = delete;
public:
	void Enter(Scene_InGame* scene);
	void Set(InGameDate date);
	void Update(float dt);

	InGameTimeMode GetTimeMode() const { return m_CurrentTimeMode; }
	void Play() { m_CurrentTimeMode = InGameTimeMode::Play; }
	void Stop() { m_CurrentTimeMode = InGameTimeMode::Stop; }

	void NewDay();

	const InGameDate& GetInGameTime() const { return m_CurrentTime; }
	float GetPassedDayTimeRatio() const;
	bool IsSunsetTimePassed() const;

private:
	InGameDate			m_CurrentTime;
	InGameTimeMode		m_CurrentTimeMode = InGameTimeMode::Stop;
	InGameTimeConfig	m_CurrentTimeConfig;

	float				m_InGameTimeMin = 0;
	float				m_RealTimeSec = 0;

	void UpdatePlay(float dt);
	void UpdateStop(float dt);

	void UpdateInGameDate();

	Scene_InGame* m_CurrInGameScene;
};

#define INGAMETIME (InGameTime::GetInstance())