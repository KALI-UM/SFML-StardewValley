#pragma once

class TileObjectSystem;
class TileModel;
class TileView;
class TileGrid;
class TileObject;

class Scene_InGame :
	public SceneBase
{
public:
	Scene_InGame();
	~Scene_InGame();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;


protected:
	TileObjectSystem*	m_TileObjectSystem;
	TileModel*			m_TileModel;
	TileView*			m_TileView;
	TileGrid*			m_TileGrid;

	TileObject* m_Back;
};

