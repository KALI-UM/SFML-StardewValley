#pragma once

class TileGrid;
class TileMapSystem;
class TileModel;
class TileController;
class TileView;
class ButtonBar;
class Scene_TileEditor :
    public SceneBase
{
public:
    Scene_TileEditor();
    ~Scene_TileEditor();

	bool Initialize() override;
	//void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	//void LateUpdate(float dt) override;
	//void FixedUpdate(float dt) override;
	void PostRender()override;
	//void Release() override;

	void ShowSceneImgui() override;
	void ViewLayerImgui();
	void CollLayerImgui();

protected:
	TileGrid*			m_TileGrid;
	TileMapSystem*		m_TileMapSystem;
	TileModel*			m_TileModel;
	TileController*		m_TileController;
	TileView*			m_TileView;
	ButtonBar*			m_ButtonBar;

	sf::RenderTexture	m_MiniMapTexture;
	SpriteObject*		m_MiniMap;

	MouseCursor*		m_Cursor;

	std::vector<std::string> m_Layers;

};

