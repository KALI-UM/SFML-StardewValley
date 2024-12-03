#include "pch.h"
#include "sf9SliceSprite.h"

sf9SliceSprite::sf9SliceSprite()
{
	m_Vertices.resize(36);
	m_Vertices.setPrimitiveType(sf::Quads);
}

void sf9SliceSprite::setTexture(const sf::Texture& tex)
{
	m_Texture = &tex;
}

const sf::Texture* sf9SliceSprite::getTexture() const
{
	return m_Texture;
}

void sf9SliceSprite::setColor(const sf::Color& color)
{
	m_Color = color;
	for (int i = 0; i < m_Vertices.getVertexCount(); i++)
	{
		m_Vertices[i].color = color;
	}
}

const sf::Color& sf9SliceSprite::getColor() const
{
	return m_Color;
}

sf::FloatRect sf9SliceSprite::getGlobalBounds() const
{
	return getTransform().transformRect(m_Vertices.getBounds());
}

sf::FloatRect sf9SliceSprite::getLocalBounds() const
{
	return m_Vertices.getBounds();
}

void sf9SliceSprite::setSize(const sf::Vector2f& size)
{
	if (size.x < m_CornerSize * 2 || size.y < m_CornerSize * 2)return;

	m_Size = size;
	sf::FloatRect unitxunit = { 0,0, m_CornerSize, m_CornerSize };

	m_Vertices[0].position = { 0,						0 };
	m_Vertices[1].position = { m_CornerSize,			0 };
	m_Vertices[2].position = { m_CornerSize,			m_CornerSize };
	m_Vertices[3].position = { 0,						m_CornerSize };

	m_Vertices[4].position = { m_CornerSize,			0 };
	m_Vertices[5].position = { m_Size.x - m_CornerSize,	0 };
	m_Vertices[6].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Vertices[7].position = { m_CornerSize,			m_CornerSize };

	m_Vertices[8].position = { m_Size.x - m_CornerSize,	0 };
	m_Vertices[9].position = { m_Size.x,				0 };
	m_Vertices[10].position = { m_Size.x,				m_CornerSize };
	m_Vertices[11].position = { m_Size.x - m_CornerSize,m_CornerSize };


	m_Vertices[12 + 0].position = { 0,						m_CornerSize };
	m_Vertices[12 + 1].position = { m_CornerSize,			m_CornerSize };
	m_Vertices[12 + 2].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Vertices[12 + 3].position = { 0,						m_Size.y - m_CornerSize };

	m_Vertices[12 + 4].position = { m_CornerSize,			m_CornerSize };
	m_Vertices[12 + 5].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Vertices[12 + 6].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Vertices[12 + 7].position = { m_CornerSize,			m_Size.y - m_CornerSize };

	m_Vertices[12 + 8].position = { m_Size.x - m_CornerSize,	m_CornerSize };
	m_Vertices[12 + 9].position = { m_Size.x,				m_CornerSize };
	m_Vertices[12 + 10].position = { m_Size.x,				m_Size.y - m_CornerSize };
	m_Vertices[12 + 11].position = { m_Size.x - m_CornerSize,m_Size.y - m_CornerSize };


	m_Vertices[24 + 0].position = { 0,						m_Size.y - m_CornerSize };
	m_Vertices[24 + 1].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Vertices[24 + 2].position = { m_CornerSize,			m_Size.y };
	m_Vertices[24 + 3].position = { 0,						m_Size.y };

	m_Vertices[24 + 4].position = { m_CornerSize,			m_Size.y - m_CornerSize };
	m_Vertices[24 + 5].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Vertices[24 + 6].position = { m_Size.x - m_CornerSize,	m_Size.y };
	m_Vertices[24 + 7].position = { m_CornerSize,			m_Size.y };

	m_Vertices[24 + 8].position = { m_Size.x - m_CornerSize,	m_Size.y - m_CornerSize };
	m_Vertices[24 + 9].position = { m_Size.x,				m_Size.y - m_CornerSize };
	m_Vertices[24 + 10].position = { m_Size.x,				m_Size.y };
	m_Vertices[24 + 11].position = { m_Size.x - m_CornerSize,m_Size.y };
}

const sf::Vector2f& sf9SliceSprite::getSize() const
{
	return m_Size;
}

void sf9SliceSprite::setCornerSize(float size)
{
	m_CornerSize = size;
	setSize(m_Size);
}

void sf9SliceSprite::setAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR, const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR, const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR)
{
	setAtlasRect(0, 0, TL);
	setAtlasRect(0, 4, TC);
	setAtlasRect(0, 8, TR);

	setAtlasRect(1, 0, ML);
	setAtlasRect(1, 4, MC);
	setAtlasRect(1, 8, MR);

	setAtlasRect(2, 0, BL);
	setAtlasRect(2, 4, BC);
	setAtlasRect(2, 8, BR);
}

void sf9SliceSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
	states.texture = m_Texture;
	target.draw(m_Vertices, states);
}

void sf9SliceSprite::setAtlasRect(int index, int start, const sf::FloatRect& rect)
{
	m_Vertices[index * 12 + 0 + start].texCoords = { rect.left,			rect.top };
	m_Vertices[index * 12 + 1 + start].texCoords = { rect.left + rect.width,	rect.top };
	m_Vertices[index * 12 + 2 + start].texCoords = { rect.left + rect.width,	rect.top + rect.height };
	m_Vertices[index * 12 + 3 + start].texCoords = { rect.left,			rect.top + rect.height };
}

