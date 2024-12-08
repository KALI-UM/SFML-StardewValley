#include "pch.h"
#include "UIObject.h"

UIObject::UIObject(const std::string& name)
	:GameObject(name)
{
}

UIObject::~UIObject()
{
}

void UIObject::SetAspectRatioPosition(const sf::Vector2f& aspect, const sf::Vector2f& offset)
{
	m_AspectPositionRatio = aspect;
	m_AspectPositionOffset = offset;
	OnWindowResize();
}

void UIObject::OnWindowResize()
{
	if (GAME_MGR->GetViewCount() == 0) return;
	sf::Vector2f viewSize = GAME_MGR->GetViewSize(SCENE_MGR->GetCurrentScene()->m_UIViewIndex);
	sf::FloatRect viewRect = GAME_MGR->GetViewRect(SCENE_MGR->GetCurrentScene()->m_UIViewIndex);

	setPosition(sf::Vector2f(viewSize.x * m_AspectPositionRatio.x + m_AspectPositionOffset.x,
		viewSize.y * m_AspectPositionRatio.y + m_AspectPositionOffset.y));
  }
