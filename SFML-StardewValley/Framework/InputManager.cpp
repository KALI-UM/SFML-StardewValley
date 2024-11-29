#include "pch.h"
#include "InputManager.h"

bool InputManager::Initialize()
{
	Clear();

	 // Horizontal
	AxisInfo infoH;
	infoH.axis = Axis::Horizontal;
	infoH.AddKey(true, sf::Keyboard::D);
	infoH.AddKey(false, sf::Keyboard::A);
	axisInfoMap.insert({ infoH.axis, infoH });

	// Vertical
	AxisInfo infoV;
	infoV.axis = Axis::Vertical;
	infoV.AddKey(true, sf::Keyboard::S);
	infoV.AddKey(false, sf::Keyboard::W);
	axisInfoMap.insert({ infoV.axis, infoV });

	m_MouseDownPosition.resize((int)sf::Mouse::ButtonCount);

	return true;
}

void InputManager::UpdateEvent(const sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::KeyPressed:
	{
		int code = ev.key.code;
		if (!Contains(heldKeys, code))
		{
			heldKeys.push_back(code);
			downKeys.push_back(code);
		}
	}
	break;
	case sf::Event::KeyReleased:
	{
		int code = ev.key.code;
		Remove(heldKeys, code);
		upKeys.push_back(code);
	}
	break;
	case sf::Event::MouseButtonPressed:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		if (!Contains(heldKeys, code))
		{
			heldKeys.push_back(code);
			downKeys.push_back(code);
		}
	}
	break;
	case sf::Event::MouseButtonReleased:
	{
		int code = sf::Keyboard::KeyCount + ev.mouseButton.button;
		Remove(heldKeys, code);
		upKeys.push_back(code);
	}
	break;
	case sf::Event::MouseMoved:
		/*mousePosition.x = ev.mouseMove.x;
		mousePosition.y = ev.mouseMove.y;*/
		break;
	}
}

void InputManager::Update(float dt)
{
	for (auto& pair : axisInfoMap)
	{
		auto& axisInfo = pair.second;
		float raw = GetAxisRaw(axisInfo.axis);
		float dir = raw;
		if (dir == 0.f && axisInfo.value != 0.f)
		{
			dir = axisInfo.value > 0.f ? -1.f : 1.f;
		}

		axisInfo.value += dir * axisInfo.sensi * dt;
		axisInfo.value = Utils::Clamp(axisInfo.value, -1.f, 1.f);

		float stopThreshold = std::abs(dir * axisInfo.sensi * dt);
		if (raw == 0.f && std::abs(axisInfo.value) < stopThreshold)
		{
			axisInfo.value = 0.f;
		}
	}
}

void InputManager::Clear()
{
	upKeys.clear();
	downKeys.clear();
}

bool InputManager::GetKeyDown(sf::Keyboard::Key key) const
{
	return Contains(downKeys, key);
}

bool InputManager::GetKeyUp(sf::Keyboard::Key key) const
{
	return Contains(upKeys, key);
}

bool InputManager::GetKey(sf::Keyboard::Key key) const
{
	return Contains(downKeys, key) || Contains(heldKeys, key);
}

sf::Vector2i InputManager::GetMousePos() const
{
	return sf::Mouse::getPosition(*GameManager::GetInstance()->GetWindow());
}

sf::Vector2f InputManager::GetMouseDefaultViewPos() const
{
	return GameManager::GetInstance()->GetWindow()->mapPixelToCoords(GetMousePos(), GameManager::GetInstance()->GetWindow()->getDefaultView());
}

sf::Vector2f InputManager::GetMouseViewPos(int index) const
{
	return GameManager::GetInstance()->GetWindow()->mapPixelToCoords(GetMousePos(), *GAME_MGR->GetView(index));
}

bool InputManager::GetMouseDown(sf::Mouse::Button btt) const
{
	return Contains(downKeys, sf::Keyboard::KeyCount + btt);
}

bool InputManager::GetMouseUp(sf::Mouse::Button btt) const
{
	return Contains(upKeys, sf::Keyboard::KeyCount + btt);

}

bool InputManager::GetMouse(sf::Mouse::Button btt) const
{
	int code = sf::Keyboard::KeyCount + btt;
	return Contains(downKeys, code) || Contains(heldKeys, code);
}

bool InputManager::GetMouseDrag(sf::Mouse::Button btt) const
{
	return (!GetMouseDown(btt))&&GetMouse(btt);
}

sf::Vector2i InputManager::GetPrevMouseDown(sf::Mouse::Button btt) const
{
	return m_MouseDownPosition[btt];
}

float InputManager::GetAxis(Axis axis) const
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
		return 0.f;
	return findIt->second.value;
}

float InputManager::GetAxisRaw(Axis axis) const
{
	auto findIt = axisInfoMap.find(axis);
	if (findIt == axisInfoMap.end())
		return 0.f;

	auto& axisInfo = findIt->second;
	auto it = heldKeys.rbegin();
	while (it != heldKeys.rend())
	{
		int code = *it;
		if (Contains(axisInfo.positives, code))
			return 1.f;
		if (Contains(axisInfo.negatives, code))
			return -1.f;

		++it;
	}
	return 0.0f;
}

bool InputManager::GetIsValidKey(int key) const
{
	return key >= 0 && key <= sf::Keyboard::KeyCount;
}

bool InputManager::GetIsValidMouse(int key) const
{
	return key >= 0 && key <= sf::Mouse::ButtonCount;
}

bool InputManager::Contains(const std::list<int>& list, int code) const
{
	return std::find(list.begin(), list.end(), code) != list.end();
}

void InputManager::Remove(std::list<int>& list, int code)
{
	list.remove(code);
}
