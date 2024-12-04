#include "pch.h"
#include "UIManager.h"
#include "UIObject.h"

void UIManager::Enter()
{	
	m_UIViewIndex = SCENE_MGR->GetCurrentScene()->m_UIViewIndex;
}

void UIManager::Update(float dt)
{
	m_UIHasFocus = false;
	for (UIObject* ui : m_UIGameObjects)
	{
		if (ui->GetHasFocus())
		{
			m_UIHasFocus= true;
			break;
		}
	}
}

void UIManager::Exit()
{
	m_UIGameObjects.clear();
}

void UIManager::RegisterUI(UIObject* ui)
{
	ui->m_UIViewIndex = m_UIViewIndex;
	m_UIGameObjects.push_back(ui);
}

sf::Vector2f UIManager::GetMouseUIViewPos() const
{
	return INPUT_MGR->GetMouseViewPos(m_UIViewIndex);
}
