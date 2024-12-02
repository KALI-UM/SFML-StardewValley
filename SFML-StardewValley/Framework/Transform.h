#pragma once

//sf::Transformable을 계층용으로 사용하기 위해 제작
//sff::Transformable은 로컬 트랜스폼으로 사용됨
namespace sf
{
	class Transformable;
}

class Transform
{
protected:
	void Initialize(sf::Transformable* transformable);
	void Initialize(const Transform& other, sf::Transformable* transformable);
public:
	void Reset();
	void ResetLocalTransform();
	void SetChild(Transform* child);
	void RemoveChild(Transform* child);
	void SetParent(Transform* parent);
	
	sf::Vector2f getPosition()const;
	sf::Vector2f getLocalPosition()const { return m_T->getPosition(); }
	void setPosition(const sf::Vector2f& pos);					//global
	void setPosition(float posx, float posy);					//global
	void setLocalPosition(const sf::Vector2f& localpos);
	void move(const sf::Vector2f& pos);
	void localMove(const sf::Vector2f& pos);

	float getRotation()const;
	float getLocalRotation()const { return m_T->getRotation(); }
	void setRotation(float rot);								//global
	void setLocalRotation(float localrot);
	void rotate(float rot);
	void localRotate(float rot);

	sf::Vector2f getScale()const;
	sf::Vector2f getLocalScale()const { m_T->getScale(); }
	void setScale(const sf::Vector2f& scale);					//global
	void setScale(float scalex, float scaley);					//global
	void setLocalScale(const sf::Vector2f& localscale);
	void scale(const sf::Vector2f& scale);
	void localScale(const sf::Vector2f& scale);

	void setOrigin(const sf::Vector2f& origin);
	void setOrigin(float originx, float originy);

	sf::Vector2f getOrigin()const { return m_T->getOrigin(); }

	const sf::Transform& getParentTransform() const;
	const sf::Transform& getTransform() const { return m_GlobalTransform; }
	const sf::Transform& getInverseTransform() const { return m_GlobalTransform.getInverse(); }
private:
	void UpdateGlobalTransform();	//로컬 트랜스폼 값을 수정한 후에 글로벌 트랜스폼 업데이트 필
	void UpdateLocalTransform();	//글로벌 트랜스폼으로 값을 수정한 후에 로컬 업데이트 필
	void UpdateChildTransform();

	static void DecomposeTransform(const sf::Transform& trans, sf::Vector2f& position, float& rotation, sf::Vector2f& scale);
	static sf::Vector2f DecomposePosition(const sf::Transform& trans);
	static float DecomposeRotation(const sf::Transform& trans, const sf::Vector2f& scale);
	static sf::Vector2f DecomposeScale(const sf::Transform& trans);

private:
	sf::Transformable*	m_T;
	sf::Transform		m_GlobalTransform;

	Transform* m_Parent = nullptr;
	std::vector<Transform*>	m_Children;
};

