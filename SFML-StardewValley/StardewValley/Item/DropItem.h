#pragma once

typedef std::string ITEMID;
typedef std::string TEXID;
class InGamePlayer;
class DropItem :
	public GameObject, public ObjectPoolable
{
public:
	bool IsNeedToReturn();
	void InitForObjectPool();
	void ResetForObjectPool();

	bool Initialize() override;
	void Reset() override;
	void Update(float dt)override;

	void Load(const ITEMID& id);
	void FloatingEffect(const sf::Vector2f& pos);

protected:
	ITEMID			m_ItemId;
	DSprite*		m_ItemIcon;
	DCircle*		m_Shadow;

	const InGamePlayer*	m_Player;
	bool			m_TaskDone = false;

	sf::Vector2f	m_StartPosition;
	sf::Vector2f	m_TargetPosition;
	float			m_Timer;
};

