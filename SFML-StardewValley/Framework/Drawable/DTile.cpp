#include "pch.h"
#include "DTile.h"

sfTile::sfTile()
{
	m_Vertices.resize(4);
	m_Vertices.setPrimitiveType(sf::Quads);
}

sfTile::~sfTile()
{
}

void sfTile::setTexture(const sf::Texture& tex)
{
	m_Texture = &tex;
}

const sf::Texture* sfTile::getTexture() const
{
	return m_Texture;
}

void sfTile::setColor(const sf::Color& color)
{
	m_Color = color;
	for (int i = 0; i < m_Vertices.getVertexCount(); i++)
	{
		m_Vertices[i].color = color;
	}
}

const sf::Color& sfTile::getColor() const
{
	return m_Color;
}

sf::FloatRect sfTile::getGlobalBounds() const
{
	return getTransform().transformRect(m_Vertices.getBounds());
}

sf::FloatRect sfTile::getLocalBounds() const
{
	return m_Vertices.getBounds();
}

void sfTile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	states.texture = m_Texture;
	target.draw(m_Vertices, states);
}

DTile::DTile()
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
}

DTile::DTile(sf::Texture* tex)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
	}
}

DTile::DTile(const std::string& filepath)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	if (filepath != "")
	{
		sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
		if (tex)
		{
			m_IsValid = true;
			m_Tile.setTexture(*tex);
		}
	}
}

DTile::DTile(const sf::Vector2f& position, sf::Texture* tex)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	m_Tile.setPosition(position);
	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
	}
}

DTile::DTile(const sf::Vector2f& position, const std::string& filepath)
	:DrawableObject(DataType::Shape, &m_Tile, &m_Tile)
{
	m_Tile.setPosition(position);
	if (filepath != "")
	{
		sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
		if (tex)
		{
			m_IsValid = true;
			m_Tile.setTexture(*tex);
		}
	}
}

DTile::DTile(const DTile& other)
	:DrawableObject(other, &m_Tile, &m_Tile), m_Tile(other.m_Tile)
{
}

DTile::DTile(DTile&& other)
	:DrawableObject(other, &m_Tile, &m_Tile), m_Tile(other.m_Tile)
{
}

DTile::~DTile()
{
}

void DTile::SetTexture(sf::Texture* tex)
{
	if (tex)
	{
		m_IsValid = true;
		m_Tile.setTexture(*tex);
	}
}

void DTile::SetTexture(const std::string& filepath)
{
	if (filepath == "")return;
	SetTexture(TEXTURE_MGR->GetByFilepath(filepath));
}

void DTile::SetTextureRect(const sf::IntRect& rect)
{
	m_Tile.m_Vertices.resize(4);
	m_Tile.m_UnitxUnit = { 0,0 };
	SetVerticesPositionByTileIndex(0, { 0,0 });
	SetVerticesTexCoordByIntRect(0, rect);
}

void DTile::SetTextureRect(const sf::IntRect& rect, const sf::Vector2u& uu)
{
	m_Tile.m_Vertices.resize(4 * (int)uu.x * (int)uu.y);
	m_Tile.m_UnitxUnit = { 0,0 };

	for (int uuy = 0; uuy < (int)uu.y; uuy++)
	{
		for (int uux = 0; uux < (int)uu.x; uux++)
		{
			SetVerticesPositionByTileIndex(uux + uuy * (int)uu.x, { uux,uuy });
			SetVerticesTexCoordByIntRect(uux + uuy * (int)uu.x,
				sf::IntRect(rect.left + rect.width / (int)uu.x * uux, rect.top + rect.height / (int)uu.y * uuy, rect.width / (int)uu.x, rect.top + rect.height / (int)uu.y));
		}
	}
}

void DTile::SetTextureRect(const std::list<sf::IntRect>& rects, const std::list<sf::Vector2i>& tiles)
{
	if (rects.size() == 0)
	{
		SetTextureRect(*rects.begin());
		return;
	}

	m_Tile.m_Vertices.resize(rects.size() * 4);
	m_Tile.m_UnitxUnit = { 0,0 };
	auto rectit = rects.begin();
	auto tileit = tiles.begin();
	for (int i = 0; i < rects.size(); i++)
	{
		SetVerticesPositionByTileIndex(i, *tileit);
		SetVerticesTexCoordByIntRect(i, *rectit);
		tileit++;
		rectit++;
	}
}

void DTile::SetUnitxUnitSize(const sf::Vector2u& uu)
{
	m_IsValid = true;
	m_Tile.m_Vertices.resize(4 * (int)uu.x * (int)uu.y);
	m_Tile.m_UnitxUnit = { 0,0 };

	for (int uuy = 0; uuy < (int)uu.y; uuy++)
	{
		for (int uux = 0; uux < (int)uu.x; uux++)
		{
			SetVerticesPositionByTileIndex(uux + uuy * (int)uu.x, { uux,uuy });
		}
	}
	SetFillColor(sf::Color::White);
}

sf::Vector2u DTile::GetTextureSize() const
{
	return m_Tile.getTexture() ? m_Tile.getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::FloatRect DTile::GetLocalBounds() const
{
	return m_Tile.getLocalBounds();
}

void DTile::SetTileOrigin(const sf::Vector2i& originIndex)
{
	setOrigin(m_Unit * (originIndex.x + 0.5f), m_Unit * (originIndex.y + 1.0f));
}

sf::Color DTile::GetColor() const
{
	return m_Tile.getColor();
}

void DTile::SetColor(const sf::Color& color)
{
	SetFillColor(color);
}

void DTile::SetColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

sf::Color DTile::GetFillColor() const
{
	return m_Tile.getColor();
}

sf::Color DTile::GetOutlineColor() const
{
	return sf::Color();
}

void DTile::SetFillColor(const sf::Color& color)
{
	m_Tile.setColor(color);
}

void DTile::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void DTile::SetOutlineColor(const sf::Color& color)
{
	//¹ÌÁö¿ø
}

void DTile::SetOutlineColor(int r, int g, int b, int a)
{
	SetOutlineColor(sf::Color(r, g, b, a));
}

void DTile::SetVerticesPositionByTileIndex(int quadIndex, const sf::Vector2i& tileIndex)
{
	m_Tile.m_UnitxUnit.x = std::max(m_Tile.m_UnitxUnit.x, (unsigned int)tileIndex.x + 1);
	m_Tile.m_UnitxUnit.y = std::max(m_Tile.m_UnitxUnit.y, (unsigned int)tileIndex.y + 1);

	m_Tile.m_Vertices[quadIndex * 4].position = sf::Vector2f(0, 0) + tileIndex.To<float>();
	m_Tile.m_Vertices[quadIndex * 4 + 1].position = sf::Vector2f(1, 0) + tileIndex.To<float>();
	m_Tile.m_Vertices[quadIndex * 4 + 2].position = sf::Vector2f(1, 1) + tileIndex.To<float>();
	m_Tile.m_Vertices[quadIndex * 4 + 3].position = sf::Vector2f(0, 1) + tileIndex.To<float>();

	m_Tile.m_Vertices[quadIndex * 4].position *= m_Unit;
	m_Tile.m_Vertices[quadIndex * 4 + 1].position *= m_Unit;
	m_Tile.m_Vertices[quadIndex * 4 + 2].position *= m_Unit;
	m_Tile.m_Vertices[quadIndex * 4 + 3].position *= m_Unit;
}

void DTile::SetVerticesTexCoordByIntRect(int quadIndex, const sf::IntRect& rect)
{
	m_Tile.m_Vertices[quadIndex * 4].texCoords = sf::Vector2f(rect.getPosition());
	m_Tile.m_Vertices[quadIndex * 4 + 1].texCoords = sf::Vector2f(rect.getPosition()) + sf::Vector2f(rect.width, 0);
	m_Tile.m_Vertices[quadIndex * 4 + 2].texCoords = sf::Vector2f(rect.getPosition()) + sf::Vector2f(rect.width, rect.height);
	m_Tile.m_Vertices[quadIndex * 4 + 3].texCoords = sf::Vector2f(rect.getPosition()) + sf::Vector2f(0, rect.height);
}
