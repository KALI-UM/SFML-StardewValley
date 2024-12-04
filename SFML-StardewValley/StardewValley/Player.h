#pragma once
#include "Tile/Tile.h"

class Tool;
enum class ItemType;
class TileObjectSystem;
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
		Attack,
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

	Tool* tool;

	std::map<std::string, AnimationClip> temp;
	std::vector<ClipInfo> clipInfos;

	Animator animator;
	ItemType itemtype;
	
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

	void GetTool(Tool* tool);

	void SetAction(Action newAction);
	void SetTileSystem(TileObjectSystem* const sys) { m_TileSystem = sys; };

	void GetItemType(ItemType type);

protected:
	Direction m_CurrDir;
	Action m_CurrAction = Action::idle;
	IsVisibleItem m_CurrEquip;

	CellIndex m_PlayerTileIndex;
	const TileObjectSystem*  m_TileSystem;
};

