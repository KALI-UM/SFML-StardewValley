#pragma once
#include "Item/Hoe.h"

class Player : public GameObject
{

public:
	enum class Direction {
		up,
		down,
		left,
		right,
	};
	enum class Action {
		idle,
		move,
		interaction,
		wateringAction,
		staminaExhausted,
	};
	enum class IsVisibleItem {
		visibleItem,
		invisibleItem,
	};

	struct ClipInfo
	{
		std::string idle;
		std::string move;
		bool flipX = false;
		sf::Vector2f point;
	};

protected:
	DSprite* body;
	DSprite* item;

	sf::Vector2f direction;
	float speed = 500.f;
	int maxStamina = 20;
	int stamina = maxStamina;

	float value = 0.f;

	Hoe* hoe;

	std::map<std::string, AnimationClip> temp;
	std::vector<ClipInfo> clipInfos;

	Animator animator;
    
	bool isActiveWeapon = false;
	//std::map<Movement, AnimationState> movementAnimations;

public:
	Player(const std::string& name);
	~Player();

	bool Initialize();
	void Reset();
	void Update(float dt);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateInter(float dt);
     

	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void PreRender();
	void PostRender();
	void ImGuiUpdate();
	void Release();

	void AnimationClips();
	
	std::string GetAnimationClipIdByDAI();

	float Staminagauge();

	Player::Direction GetDirection();

	Player::IsVisibleItem GetIsVisibleItem();
	
	void GetHoe(Hoe* hoe);
	void SetAction(Action newAction);

protected:
	Direction m_CurrDir;
	Action m_CurrAction = Action::idle;
	IsVisibleItem m_CurrEquip;
};

