#pragma once

class TileObjectSystem;
class TileModel;
class TileView;
class TileGrid;

class Scene_FarmHouseIndoor :
	public SceneBase
{
	Scene_FarmHouseIndoor();
	~Scene_FarmHouseIndoor();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;

	TileObjectSystem* m_TileObjectSystem;
	TileModel* m_TileModel;
	TileView* m_TileView;
	TileGrid* m_TileGrid;
};

