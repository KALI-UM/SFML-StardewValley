#pragma once

class sfTile :public sf::Drawable, public sf::Transformable
{
public:
	sfTile();
	~sfTile();
	sf::VertexArray m_Vertices;
	sf::Color m_Color;
	const sf::Texture* m_Texture;
	sf::Vector2u m_UnitxUnit;		//NxM 鸥老

	void setTexture(const sf::Texture& tex);
	const sf::Texture* getTexture() const;
	void setColor(const sf::Color& color);
	const sf::Color& getColor()const;
	sf::FloatRect getGlobalBounds()const;
	sf::FloatRect getLocalBounds()const;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class DTile :
	public DrawableObject
{
public:
	DTile();
	DTile(sf::Texture* tex);
	DTile(const std::string& filepath);
	DTile(const sf::Vector2f& position, sf::Texture* tex);
	DTile(const sf::Vector2f& position, const std::string& filepath);
	DTile(const DTile& other);
	DTile(DTile&& other);
	virtual ~DTile();

	const sf::Vector2u& GetUnitxUnit()const { return m_Tile.m_UnitxUnit; };

	void SetTexture(sf::Texture* tex);
	void SetTexture(const std::string& filepath);
	void SetTextureRect(const sf::IntRect& rect);
	void SetTextureRect(const sf::IntRect& rect, const sf::Vector2u& uu);
	void SetTextureRect(const std::list<sf::IntRect>& rects, const std::list<sf::Vector2i>& tiles);
	void SetUnitxUnitSize(const sf::Vector2u& uu);

	sf::Vector2u GetTextureSize()const;
	sf::FloatRect GetLocalBounds()const;
	void SetTileOrigin(const sf::Vector2i& originIndex);

	sf::Color GetColor() const;								//GetFillColor客 悼老		
	void SetColor(const sf::Color& color);					//SetFillColor客 悼老
	void SetColor(int r, int g, int b, int a = 255);		//SetFillColor客 悼老
	sf::Color GetFillColor() const;
	sf::Color GetOutlineColor() const;
	void SetFillColor(const sf::Color& color);
	void SetFillColor(int r, int g, int b, int a = 255);
	void SetOutlineColor(const sf::Color& color);
	void SetOutlineColor(int r, int g, int b, int a = 255);
	
	void SetUnitSize(float unit) { m_Unit = unit; }
	float GetUnitSize()const { return m_Unit; }

	void SetVerticesPositionByTileIndex(int quadIndex, const sf::Vector2i& tileIndex);
	void SetVerticesTexCoordByIntRect(int quadIndex, const sf::IntRect& rect);
private:
	float			m_Unit = 16;
	sfTile			m_Tile;

};

