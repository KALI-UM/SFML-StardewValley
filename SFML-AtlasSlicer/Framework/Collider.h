#pragma once
#include "Transform.h"

class GameObject;
enum class CollisionDetectionMode
{
	Discrete,					//�ͳθ���������
	Continuous,					//�������� �ʴ� ��ü �ͳθ�������
	ContinuousDynamic,			//�����̴� ��ü�� �ͳθ�������
};
class Collider : public Transform
{
public:
	Collider();
	~Collider();

	bool INITIALIZE();
	void FIXEDUPDATE(float dt);

	void SetCollisionDetectionMode(CollisionDetectionMode mode) { m_Mode = mode; };

	GameObject* GetGameObject() { return m_Owner; }
	void SetCollisionEnter(std::function<void(Collider*)> func) { m_CollisionEnterFunc; };
	void SetCollisionStay(std::function<void(Collider*)> func) { m_CollisionStayFunc; };
	void SetCollisionExit(std::function<void(Collider*)> func) { m_CollisionExitFunc; };
protected:
	GameObject* m_Owner;

	CollisionDetectionMode m_Mode = CollisionDetectionMode::Discrete;
	sf::Vector2f m_Direction;
	sf::Vector2f m_PrevPosition;

	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float dt) = 0;
private:
	bool m_IsTrigger;

	void OnCollisionEnter(Collider* collider);
	void OnCollisionStay(Collider* collider);
	void OnCollisionExit(Collider* collider);
	std::function<void(Collider*)> m_CollisionEnterFunc;
	std::function<void(Collider*)> m_CollisionStayFunc;
	std::function<void(Collider*)> m_CollisionExitFunc;

	void OnTriggerEnter(Collider* collider);
	void OnTriggerStay(Collider* collider);
	void OnTriggerExit(Collider* collider);
	std::function<void(Collider*)> m_TriggerEnterFunc;
	std::function<void(Collider*)> m_TriggerStayFunc;
	std::function<void(Collider*)> m_TriggerExitFunc;
};

