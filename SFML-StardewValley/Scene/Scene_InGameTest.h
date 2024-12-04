#pragma once

class TileObjectSystem;
class TileModel;
class TileController;
class TileView;
class TileGrid;

class Player;
class PlayerStatusUi;
class Tool;

class TileObject;

class Scene_InGameTest :
    public SceneBase
{
public:
	Scene_InGameTest();
	~Scene_InGameTest();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;



protected:
	TileObjectSystem* m_TileObjectSystem;
	TileModel* m_TileModel;
	TileView* m_TileView;
	TileGrid* m_TileGrid;

	Player* m_Player;
	Animator m_PlayerAnimator;
	PlayerStatusUi* m_PlayerStatusUi;
	Tool* m_TestItem;


	TileObject* m_TestTObj;
};

