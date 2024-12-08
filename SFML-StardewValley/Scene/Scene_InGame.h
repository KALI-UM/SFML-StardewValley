#pragma once

class TileObjectSystem;
class TileModel;
class TileView;
class TileGrid;
class TileObject;

class Player;

class Inventory;
class InventoryUI;
class EventUI;
class GameInfoUI;

enum class InGamePlayMode
{
	Play,
	Stop,
	PlayCutScene,
	PlayChangeScene,
	Debug,
};

class Scene_InGame :
	public SceneBase
{
public:
	Scene_InGame(const std::string& name);
	~Scene_InGame();

	bool Initialize() override;
	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	void OnWindowResize()override;

	bool GetIsSunSet()const { return m_IsSunset; }

	void SetPlayMode(InGamePlayMode mode);
	InGamePlayMode GetPlayMode() const { return m_CurrPlayMode; }
	EventUI* GetPopUpUI() { return m_PopUpUI; };

	void EndDay();
protected:
	InGamePlayMode m_CurrPlayMode = InGamePlayMode::Play;

	void UpdatePlayMode(float dt);
	virtual void UpdatePlay(float dt);
	virtual void UpdateStop(float dt);
	virtual void UpdateCutScene(float dt);
	virtual void UpdateChangeScene(float dt);
	virtual void UpdateDebug(float dt);

	void DebugInputUpdate(float dt);

private:
	float m_CutSceneTimer;

protected:
	sf::Vector2f		m_MapSize;
	sf::Color			m_SunsetLightColor = sf::Color(10, 10, 80, 150);
	bool				m_IsSunset = false;
	bool				m_IsPlayerInHouse = false;

	TileObjectSystem* m_TileObjectSystem;
	TileModel* m_TileModel;
	TileView* m_TileView;
	TileGrid* m_TileGrid;

	std::string			m_TerrainFilepath;
	TileObject* m_Back;
	TileObject* m_Front;

	Player* m_Player;

	Inventory* m_Inventory;
	InventoryUI* m_InventoryUI;

	EventUI* m_PopUpUI;
	GameInfoUI* m_GameInfoUI;
};

