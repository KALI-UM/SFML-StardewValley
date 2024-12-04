#pragma once

enum class Action;

typedef sf::Vector2i CellIndex;

class D9SliceSprite;
class SubButtonBar;
class DTile;
class ButtonBar
	:public UIObject
{
public:
	ButtonBar();
	~ButtonBar();

	bool Initialize() override;
	void Reset()override;
	void Update(float dt)override;
	void Release()override;

	void SetButtonTex(const CellIndex& bttIndex, const std::string& texId);
	DTile* const GetCurrButtonTile() const { return m_CurrTile; }
	const std::list<std::string>& GetCurrTexIds();

protected:
	std::string m_BarTexId;

	D9SliceSprite*		m_Bar;
	DTile*				m_CurrTile;
	std::list<CellIndex>	m_SelectingButtons;
	std::list<sf::IntRect>	m_CurrCoords;
	std::list<CellIndex>	m_CurrIndices;
	std::list<std::string>	m_CurrTexIds;

	std::vector<std::vector<DTile*>> m_Buttons;
	std::vector<std::vector<std::string>> m_ButtonTexIds;
	std::vector<std::vector<sf::IntRect>> m_ButtonTexCoords;

	sf::Vector2f m_PrevMouseDown;

};

