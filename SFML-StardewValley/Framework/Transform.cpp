#include "pch.h"
#include "Transform.h"

void Transform::Initialize(sf::Transformable* transformable)
{
	m_T = transformable;
	if (!m_T)m_T = new sf::Transformable();
	Reset();
}

void Transform::Initialize(const Transform& other, sf::Transformable* transformable)
{
	m_T = transformable;
	if (!m_T)m_T = new sf::Transformable();
}

void Transform::Reset()
{
	ResetLocalTransform();
}

void Transform::ResetLocalTransform()
{
	m_T->setPosition(0, 0);
	m_T->setRotation(0);
	m_T->setScale(1, 1);
	UpdateGlobalTransform();
}

void Transform::SetChild(Transform* child)
{
	child->m_Parent = this;
	m_Children.push_back(child);
	child->UpdateGlobalTransform();
}

void Transform::RemoveChild(Transform* child)
{
	for (auto t = m_Children.begin(); t != m_Children.end(); t++)
	{
		if (*t == child)
		{
			child->m_Parent = nullptr;
			child->UpdateGlobalTransform();
			m_Children.erase(t);
			break;
		}
	}
}

void Transform::SetParent(Transform* parent)
{
	if (m_Parent)
	{
		m_Parent->RemoveChild(this);
	}
	m_Parent = parent;
	if (m_Parent)
		m_Parent->SetChild(this);
}

sf::Vector2f Transform::getPosition() const
{
	return DecomposePosition(m_GlobalTransform);
}


void Transform::setPosition(const sf::Vector2f& pos)
{
	sf::Vector2f globalPosition = DecomposePosition(m_GlobalTransform);
	sf::Vector2f globalScale = DecomposeScale(m_GlobalTransform);
	float globalRotation = DecomposeRotation(m_GlobalTransform, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.translate(pos).rotate(globalRotation).scale(globalScale);
	UpdateLocalTransform();
}

void Transform::setPosition(float posx, float posy)
{
	setPosition(sf::Vector2f(posx, posy));
}

void Transform::setLocalPosition(const sf::Vector2f& localpos)
{
	m_T->setPosition(localpos);
	UpdateGlobalTransform();
}

void Transform::move(const sf::Vector2f& pos)
{
	sf::Vector2f globalPosition;
	float globalRotation;
	sf::Vector2f globalScale;
	DecomposeTransform(m_GlobalTransform, globalPosition, globalRotation, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.translate(globalPosition + pos).rotate(globalRotation).scale(globalScale);
	UpdateLocalTransform();
}

void Transform::localMove(const sf::Vector2f& pos)
{
	m_T->move(pos);
	UpdateGlobalTransform();
}

float Transform::getRotation() const
{
	return DecomposeRotation(m_GlobalTransform, getScale());
}

void Transform::setRotation(float rot)
{
	sf::Vector2f globalPosition = DecomposePosition(m_GlobalTransform);
	sf::Vector2f globalScale = DecomposeScale(m_GlobalTransform);
	float globalRotation = DecomposeRotation(m_GlobalTransform, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.sf::Transform::translate(globalPosition).sf::Transform::rotate(rot).sf::Transform::scale(globalScale);
	UpdateLocalTransform();
}

void Transform::setLocalRotation(float localrot)
{
	m_T->setRotation(localrot);
	UpdateGlobalTransform();
}

void Transform::rotate(float rot)
{
	sf::Vector2f globalPosition;
	float globalRotation;
	sf::Vector2f globalScale;
	DecomposeTransform(m_GlobalTransform, globalPosition, globalRotation, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.sf::Transform::translate(globalPosition).sf::Transform::rotate(globalRotation + rot).sf::Transform::scale(globalScale);
	UpdateLocalTransform();
}

void Transform::localRotate(float rot)
{
	m_T->rotate(rot);
	UpdateGlobalTransform();
}

sf::Vector2f Transform::getScale() const
{
	return DecomposeScale(m_GlobalTransform);
}

void Transform::setScale(const sf::Vector2f& scale)
{
	sf::Vector2f globalPosition = DecomposePosition(m_GlobalTransform);
	sf::Vector2f globalScale = DecomposeScale(m_GlobalTransform);
	float globalRotation = DecomposeRotation(m_GlobalTransform, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.sf::Transform::translate(globalPosition).sf::Transform::rotate(globalRotation).sf::Transform::scale(scale);

	UpdateLocalTransform();
}

void Transform::setScale(float scalex, float scaley)
{
	setScale(sf::Vector2f(scalex, scaley));
}

void Transform::setLocalScale(const sf::Vector2f& localscale)
{
	m_T->setScale(localscale);
	UpdateGlobalTransform();
}

void Transform::scale(const sf::Vector2f& scale)
{
	sf::Vector2f globalPosition = DecomposePosition(m_GlobalTransform);
	sf::Vector2f globalScale = DecomposeScale(m_GlobalTransform);
	float globalRotation = DecomposeRotation(m_GlobalTransform, globalScale);
	m_GlobalTransform = sf::Transform::Identity;
	m_GlobalTransform.sf::Transform::translate(globalPosition).sf::Transform::rotate(globalRotation).sf::Transform::scale(sf::Vector2f(globalScale.x*scale.x, globalScale.y * scale.y));
	UpdateLocalTransform();
}

void Transform::localScale(const sf::Vector2f& scale)
{
	m_T->scale(scale);
	UpdateGlobalTransform();
}

void Transform::setOrigin(const sf::Vector2f& origin)
{
	m_T->setOrigin(origin);
}

void Transform::setOrigin(float originx, float originy)
{
	setOrigin(sf::Vector2f(originx, originy));
}

const sf::Transform& Transform::getParentTransform() const
{
	if (m_Parent)
		return m_Parent->getTransform();
	else
		return sf::Transform::Identity;
}

void Transform::UpdateGlobalTransform()
{
	m_GlobalTransform = getParentTransform() * m_T->getTransform();
	UpdateChildTransform();
}

void Transform::UpdateLocalTransform()
{
	sf::Transform newLocalTransform = m_GlobalTransform*getParentTransform().getInverse();
	sf::Vector2f newPosition= DecomposePosition(newLocalTransform);
	sf::Vector2f newScale=DecomposeScale(newLocalTransform);
	float newRotation=DecomposeRotation(newLocalTransform, newScale);
	m_T->setPosition(newPosition);
	m_T->setRotation(newRotation);
	m_T->setScale(newScale);
	UpdateChildTransform();
}

void Transform::UpdateChildTransform()
{
	for (auto& child : m_Children)
	{
		child->UpdateGlobalTransform();
	}
}

void Transform::DecomposeTransform(const sf::Transform& trans, sf::Vector2f& position, float& rotation, sf::Vector2f& scale)
{
	position = DecomposePosition(trans);
	scale = DecomposeScale(trans);
	rotation = DecomposeRotation(trans, scale);
}

sf::Vector2f Transform::DecomposePosition(const sf::Transform& trans)
{
	const float* matrix = trans.getMatrix();
	return sf::Vector2f(matrix[12], matrix[13]);
}

float Transform::DecomposeRotation(const sf::Transform& trans, const sf::Vector2f& scale)
{
	const float* matrix = trans.getMatrix();
	float rotation;
	//rotation 값 추출
	float normalizedA = matrix[0] / scale.x;
	float normalizedB = matrix[4] / scale.x;
	rotation = Utils::Angle({ normalizedA, normalizedB });

	return rotation;
}

sf::Vector2f Transform::DecomposeScale(const sf::Transform& trans)
{
	const float* matrix = trans.getMatrix();
	sf::Vector2f scale;
	//scale 크기 추출
	scale.x = Utils::Magnitude(sf::Vector2f(matrix[0], matrix[4]));
	scale.y = Utils::Magnitude(sf::Vector2f(matrix[1], matrix[5]));

	//행렬식의 determinant...? 왓더로 음수 여부 추출
	if ((matrix[0] * matrix[5] - matrix[4] * matrix[1]) < 0)
	{
		scale.x = -scale.x;
	}
	return scale;
}

