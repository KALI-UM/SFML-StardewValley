#pragma once
#include "Item/Tool.h"
#include "Animator.h"

class Hoe :
    public Tool
{
protected:
    Animator animator;
    Animator effectAnimator;
    DSprite* sprite;
    DSprite* effectSprite;
    Player* player;

    std::map<std::string, AnimationClip> temp;
    std::map<std::string, AnimationClip> temp2;

    float count = 0.f;
    float time = 0.f;
    float delay = 0.65f;
    
    float value = 0.f;
    float speed = 0.f;

    
public:

    Hoe();
    ~Hoe();

    void ToolAction(Player* const player);

    bool Initialize();

    void AnimationClips();
    void Update(float dt);
    void GetPlayer(Player* player);
};

