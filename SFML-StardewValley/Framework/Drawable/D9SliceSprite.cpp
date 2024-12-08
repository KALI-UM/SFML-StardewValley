#include "pch.h"
#include "D9SliceSprite.h"

D9SliceSprite::D9SliceSprite(sf::Texture* tex)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

D9SliceSprite::D9SliceSprite(const std::string& filepath)
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
	sf::Texture* tex = ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath);
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

D9SliceSprite::D9SliceSprite()
	:DrawableObject(DataType::Sprite, &m_Sprite, &m_Sprite)
{
}

D9SliceSprite::D9SliceSprite(const D9SliceSprite& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

D9SliceSprite::D9SliceSprite(D9SliceSprite&& other)
	:DrawableObject(other, &m_Sprite, &m_Sprite), m_Sprite(other.m_Sprite)
{
}

D9SliceSprite::~D9SliceSprite()
{
}

sf::FloatRect D9SliceSprite::GetLocalBounds() const
{
	return m_Sprite.getLocalBounds();
}

void D9SliceSprite::SetTexture(sf::Texture* tex)
{
	if (tex)
	{
		m_IsValid = true;
		m_Sprite.setTexture(*tex);
	}
}

void D9SliceSprite::SetTexture(const std::string& filepath)
{
	SetTexture(ResourceManager<sf::Texture>::GetInstance()->GetByFilepath(filepath));
}

void D9SliceSprite::SetOrigin(OriginType type, const sf::Vector2f& detail)
{
	setOrigin(((GetLocalBounds().width / 2) * ((int)type % 3)) + detail.x,
		((GetLocalBounds().height / 2) * ((int)type / 3)) + detail.y);
}

sf::Vector2u D9SliceSprite::GetTextureSize() const
{
	return m_Sprite.getTexture() ? m_Sprite.getTexture()->getSize() : sf::Vector2u(0, 0);
}

sf::Color D9SliceSprite::GetColor() const
{
	return m_Sprite.getColor();
}

void D9SliceSprite::SetColor(const sf::Color& color)
{
	m_Sprite.setColor(color);
}

void D9SliceSprite::SetColor(int r, int g, int b, int a)
{
	SetColor(sf::Color(r, g, b, a));
}

sf::Color D9SliceSprite::GetFillColor() const
{
	return GetColor();
}

sf::Color D9SliceSprite::GetOutlineColor() const
{
	//return GetColor();
	return sf::Color::Transparent;
}

void D9SliceSprite::SetFillColor(const sf::Color& color)
{
	SetColor(color);
}

void D9SliceSprite::SetFillColor(int r, int g, int b, int a)
{
	SetFillColor(sf::Color(r, g, b, a));
}

void D9SliceSprite::SetOutlineColor(const sf::Color& color)
{
	//SetColor(color);
}

void D9SliceSprite::SetOutlineColor(int r, int g, int b, int a)
{
	//SetOutlineColor(sf::Color(r, g, b, a));
}

void D9SliceSprite::SetSize(const sf::Vector2f& size)
{
	m_Sprite.setSize(size);
}

void D9SliceSprite::SetCornerSize(float size)
{
	m_Sprite.setCornerSize(size);
}

void D9SliceSprite::SetAtlas(const sf::FloatRect& TL, const sf::FloatRect& TC, const sf::FloatRect& TR, const sf::FloatRect& ML, const sf::FloatRect& MC, const sf::FloatRect& MR, const sf::FloatRect& BL, const sf::FloatRect& BC, const sf::FloatRect& BR)
{
	m_Sprite.setAtlas(	TL, TC, TR,
						ML, MC, MR,
						BL, BC, BR);
}

