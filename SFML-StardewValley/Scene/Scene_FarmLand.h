#pragma once

#include "Scene_InGame.h"
class Scene_FarmLand :
    public Scene_InGame
{
public:
	Scene_FarmLand();
	~Scene_FarmLand();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;
	void Exit() override;

protected:

};
