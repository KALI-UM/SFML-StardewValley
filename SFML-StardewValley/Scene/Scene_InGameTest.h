#pragma once

class TileObjectSystem;
class TileModel;
class TileController;
class TileView;
class TileGrid;

class Player;
class PlayerStatusUi;
class Hoe;

class Scene_InGameTest :
    public SceneBase
{
public:
	Scene_InGameTest();
	~Scene_InGameTest();

	bool Initialize() override;
	//void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	void PostRender()override;
	//void Release() override;


protected:
	TileObjectSystem* m_TileObjectSystem;
	TileModel* m_TileModel;
	TileView* m_TileView;
	TileGrid* m_TileGrid;

	Player* m_Player;
	Animator m_PlayerAnimator;
	PlayerStatusUi* m_PlayerStatusUi;
	Hoe* m_TestItem;
};

