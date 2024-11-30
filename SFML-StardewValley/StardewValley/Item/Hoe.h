#pragma once
#include "Item/Tool.h"
#include "Animator.h"

class Hoe :
    public Tool
{
protected:
    Animator animator;
    DSprite* sprite;

    std::map<std::string, AnimationClip> temp;

    float count = 0.f;
    float time = 0.f;
    float delay = 2.f;

public:

    Hoe();
    ~Hoe();

    void ToolAction(Player* const player);

    bool Initialize();

    void AnimationClips();
    void Update(float dt);
};

