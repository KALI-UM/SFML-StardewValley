#pragma once

class TileMapSystem;
class TileModel;
class TileController;
class TileView;
class TileGrid;

class Player;
class PlayerStatusUi;
class Tool;

class Scene_InGameTest:
    public SceneBase
{
public:
	Scene_InGameTest();
	~Scene_InGameTest();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;



protected:
	TileMapSystem* m_TileMapSystem;
	TileModel* m_TileModel;
	TileController* m_TileController;
	TileView* m_TileView;
	TileGrid* m_TileGrid;

	Player* m_Player;
	Animator m_PlayerAnimator;
	PlayerStatusUi* m_PlayerStatusUi;
	Tool* m_TestItem;
};

