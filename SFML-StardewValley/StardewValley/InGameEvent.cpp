#include "pch.h"
#include "InGameEvent.h"
#include "Scene_InGame.h"
#include "EventUI.h"

void InGameEvent::Enter(Scene_InGame* scene)
{
	m_CurrInGameScene = scene;
}

void InGameEvent::EVENT(const std::string& subtype)
{
	auto it = subtype.find("#");
	if (it != std::string::npos)
	{
		std::cout << "EVENT : " << subtype << std::endl;

		std::string EVENT = subtype.substr(0, it);
		if (EVENT == "ENTER")
		{
			ENTER(subtype.substr(it + 1, subtype.length() - (it + 1)));
		}

		if (EVENT == "ENDDAY")
		{
			ENDDAY();
		}
	}
}

void InGameEvent::ENTER(const std::string& scene)
{
	auto it = scene.find("#");
	if (it != std::string::npos)
	{
		std::string pos = scene.substr(0, it);
		int index = pos.find(",");
		int x = std::stoi(pos.substr(0, index));
		int y = std::stoi(pos.substr(index + 1, pos.length() - (index + 1)));
		InGameEvent::m_EnterPoint = { x,y };
	}
	SCENE_MGR->ChangeScene("InGame::" + scene.substr(it + 1, scene.length() - (it + 1)));
}

void InGameEvent::ENDDAY()
{
	if (m_DelayTimer != -1&& m_DelayTimer + 2 > FRAMEWORK->GetRealTime())
	{
		return;
	}

	if (m_CurrInGameScene->GetPlayMode() == InGamePlayMode::PlayCutScene|| !m_CurrInGameScene->GetIsSunSet())
	{
		return;
	}

	EventUI* popupui = m_CurrInGameScene->GetPopUpUI();
	popupui->SetIsActive(true);
	popupui->SetEventString(L"오늘은 이만 잠들까요?");
	popupui->SetOKButtonFunc([=]() {popupui->RESET(); this->m_DelayTimer = FRAMEWORK->GetRealTime(); m_CurrInGameScene->EndDay(); m_CurrInGameScene->Save();  });
	popupui->SetNOButtonFunc([=]() { popupui->RESET(); this->m_DelayTimer= FRAMEWORK->GetRealTime(); });

}