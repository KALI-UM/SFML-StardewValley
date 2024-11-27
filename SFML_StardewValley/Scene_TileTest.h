#pragma once

class TileGrid;
class TileModel;
class TileView;
class Scene_TileTest :
    public SceneBase
{
public:
    Scene_TileTest();
    ~Scene_TileTest();

	bool Initialize() override;
	//void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	//void ShowSceneImgui() override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	//void Release() override;

protected:
	TileGrid* m_TileGrid;

	TileModel* m_TileModel;
	TileView* m_TileView;

};

