#pragma once
#include "Singleton.h"
#include <bitset>

enum class Axis
{
	Horizontal = 0,
	Vertical = 1,
};

struct AxisInfo
{
	Axis axis;
	std::list<int> positives;		// 1.0f
	std::list<int> negatives;		// -1.0f
	float sensi = 10.f;
	float value = 0.f;

	void AddKey(bool positive, int code)
	{
		if (positive)
			positives.push_back(code);
		else
			negatives.push_back(code);
	}
};

class InputManager :
	public Singleton<InputManager>
{
public:
	bool Initialize();
	void UpdateEvent(const sf::Event& ev);
	void Update(float dt);
	void Clear();

	bool GetKeyDown(sf::Keyboard::Key key) const;
	bool GetKeyUp(sf::Keyboard::Key key) const;
	bool GetKey(sf::Keyboard::Key key) const;

	sf::Vector2i GetMousePos()const;					//스크린 좌표계 기준
	sf::Vector2f GetMouseDefaultViewPos() const;		//기본뷰 좌표계 기준
	sf::Vector2f GetMouseViewPos(int index) const;

	bool GetMouseDown(sf::Mouse::Button btt) const;
	bool GetMouseUp(sf::Mouse::Button btt) const;
	bool GetMouse(sf::Mouse::Button btt) const;
	bool GetMouseDrag(sf::Mouse::Button btt)const;

	sf::Vector2i GetPrevMouseDown(sf::Mouse::Button btt) const;

	float GetAxis(Axis axis) const;
	float GetAxisRaw(Axis axis) const;
private:

	bool GetIsValidKey(int key) const;
	bool GetIsValidMouse(int key) const;

	bool Contains(const std::list<int>& list, int code) const;
	void Remove(std::list<int>& list, int code);

	std::list<int> downKeys;
	std::list<int> heldKeys;
	std::list<int> upKeys;

	std::unordered_map<Axis, AxisInfo> axisInfoMap;

	std::vector<sf::Vector2i>						m_MouseDownPosition;
};

#define MOUSEPOS (INPUT_MGR->GetMouseDefaultViewPos())