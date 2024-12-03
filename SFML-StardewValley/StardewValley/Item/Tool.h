#pragma once
#include "Item/Item.h"

class Player;
class Tool :
	public Item
{
protected:

	Animator animator;
	Animator effectAnimator;
	DSprite* sprite;
	DSprite* effectSprite;
	Player* player;

	std::map<std::string, AnimationClip> temp;

public:
	Tool(const ITEMID& itemId);
	~Tool() {};

	void Use(Player* const player);

	int GetEnergyCost() const { return m_EnergyCost; }

	bool Initialize();

	void AnimationClips();
	void Update(float dt);
	void GetPlayer(Player* player);
protected:

	int m_EnergyCost = 2;
};

