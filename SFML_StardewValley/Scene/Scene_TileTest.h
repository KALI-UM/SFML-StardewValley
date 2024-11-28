#pragma once

class TileGrid;
class TileMapSystem;
class TileModel;
class TileController;
class TileView;
class ButtonBar;
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

	void ShowSceneImgui() override;

protected:
	TileGrid*			m_TileGrid;
	TileMapSystem*		m_TileMapSystem;
	TileModel*			m_TileModel;
	TileController*		m_TileController;
	TileView*			m_TileView;
	ButtonBar*			m_ButtonBar;

	std::vector<std::string> m_Layers;

};

