#pragma once

class Player;
class PlayerStatusUi;
class Hoe;

class Scene_Test:
	public SceneBase
{
public:
	Scene_Test();
	~Scene_Test();

	bool Initialize() override;
	void Reset() override;
	void Enter() override;
	void Update(float dt) override;

	Player* temp;
	Animator animator;
	PlayerStatusUi* statusUi;
	Hoe* hoe;
};

