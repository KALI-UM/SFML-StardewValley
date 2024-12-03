#pragma once

class TileMapSystem;
class TileModel;
class TileController;
class TileView;
class TileGrid;

class Player;
class PlayerStatusUi;
class Item;

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
	TileMapSystem* m_TileMapSystem;
	TileModel* m_TileModel;
	TileController* m_TileController;
	TileView* m_TileView;
	TileGrid* m_TileGrid;

	Player* m_Player;
	Animator m_PlayerAnimator;
	PlayerStatusUi* m_PlayerStatusUi;
	Item* m_TestItem;
};

