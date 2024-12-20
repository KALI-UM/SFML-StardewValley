#pragma once
#include "Scene_InGame.h"
class Scene_Road :
    public Scene_InGame
{
public:
	Scene_Road();
	~Scene_Road();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
protected:

};