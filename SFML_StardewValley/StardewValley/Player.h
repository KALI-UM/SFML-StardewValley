#pragma once

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

	std::map<std::string, AnimationClip> temp;
	std::vector<ClipInfo> clipInfos;
	ClipInfo* currentClipInfo = nullptr;

	Animator animator;
    
	bool isActiveWeapon = false;
	//std::map<Movement, AnimationState> movementAnimations;

public:
	Player();
	~Player();

	bool Initialize();
	void Reset();
	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void PreRender();
	void PostRender();
	void ImGuiUpdate();
	void Release();

	void AnimationClips();
	
	std::string GetAnimationClipIdByDAI();

protected:
	Direction m_CurrDir;
	Action m_CurrAction;
	IsVisibleItem m_CurrEquip;
};

