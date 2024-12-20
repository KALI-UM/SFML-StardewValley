#pragma once
#include "Scene_InGame.h"

class Scene_FarmHouseIndoor :
	public Scene_InGame
{
public:
	Scene_FarmHouseIndoor();
	~Scene_FarmHouseIndoor();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
};

