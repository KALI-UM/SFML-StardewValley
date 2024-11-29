#pragma once

class DShape :
	public DrawableObject
{
protected:
	DShape(sf::Shape* shape, const sf::Color& line, float thick = 1.0f, const sf::Color& fill = sf::Color::Transparent);
	DShape(sf::Shape* shape, sf::Texture* tex);
	DShape(const DShape& other);
	DShape(DShape&& other);
	virtual ~DShape();
public:
	void SetTexture(sf::Texture* tex);
	sf::Vector2u GetTextureSize() const;

	sf::Color GetColor() const;								//GetFillColor�� ����		
	void SetColor(const sf::Color& color);					//SetFillColor�� ����
	void SetColor(int r, int g, int b, int a = 255);		//SetFillColor�� ����
	sf::Color GetFillColor() const;
	sf::Color GetOutlineColor() const;
	void SetFillColor(const sf::Color& color);
	void SetFillColor(int r, int g, int b, int a = 255);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineColor(int r, int g, int b, int a = 255);
	void SetOutlineThickness(float thick);
private:
	sf::Shape* m_Shape;
};

