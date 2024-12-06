#include "pch.h"
#include "InGameTime.h"

void InGameTime::Set(InGameDate date)
{
	m_CurrentTime = date;
}

void InGameTime::Update(float dt)
{
	//std::cout << m_CurrentTime.hour << ":" << m_CurrentTime.min << std::endl;

	switch (m_CurrentTimeMode)
	{
	case InGameTimeMode::Play:
		UpdatePlay(dt);
		break;
	case InGameTimeMode::Stop:
		UpdateStop(dt);
		break;
	}
}

void InGameTime::NewDay()
{
	m_CurrentTime.day++;
	m_CurrentTime.hour = m_CurrentTimeConfig.wakeUpTimeHour;
	m_CurrentTime.min = 0;

	m_RealTimeSec = 0;
}

float InGameTime::GetPassedDayTimeRatio() const
{
	return ((m_CurrentTime.hour - m_CurrentTimeConfig.wakeUpTimeHour) * 60.f + (float)m_CurrentTime.min)
		/ (m_CurrentTimeConfig.hourPerDay * 60.0f);
}

bool InGameTime::IsSunsetTimePassed() const
{
	return m_CurrentTime.hour >= m_CurrentTimeConfig.sunSetTimeHour;
}

void InGameTime::UpdatePlay(float dt)
{
	m_RealTimeSec += dt;
	m_InGameTimeMin = m_RealTimeSec / m_CurrentTimeConfig.realTimePerMinute;

	UpdateInGameDate();
}

void InGameTime::UpdateStop(float dt)
{
}

void InGameTime::UpdateInGameDate()
{
	m_CurrentTime.hour = m_CurrentTimeConfig.wakeUpTimeHour + (int)m_InGameTimeMin / 60;
	m_CurrentTime.min = (int)m_InGameTimeMin % 60;
}


