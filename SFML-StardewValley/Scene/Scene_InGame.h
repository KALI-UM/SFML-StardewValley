#pragma once

class TileObjectSystem;
class TileModel;
class TileView;
class TileGrid;
class TileObject;

class InGamePlayer;

class Inventory;
class InventoryUI;
class EventUI;
class GameInfoUI;

class DropItem;

class Tool;

namespace sf
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(sf::Vector2i, x, y);
}

struct SceneRawFile
{
	std::string terrainFile;
	struct TObjInfo
	{
		std::string layer;
		sf::Vector2i index;
		std::string tobjId;
		NLOHMANN_DEFINE_TYPE_INTRUSIVE(TObjInfo, layer, index, tobjId);
	};

	std::vector<TObjInfo> tileObjects;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SceneRawFile, terrainFile, tileObjects);
};

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
	void Reset() override;
	void Exit() override;
	void OnWindowResize()override;

	bool GetIsSunSet()const { return m_IsSunset; }
	void SetPlayMode(InGamePlayMode mode);
	InGamePlayMode GetPlayMode() const { return m_CurrPlayMode; }
	EventUI* GetPopUpUI() { return m_PopUpUI; };
	void EndDay();

	ObjectPool <DropItem> m_DropItems;
	const InGamePlayer* GetPlayer() const { return m_Player; }
protected:
	InGamePlayMode m_CurrPlayMode = InGamePlayMode::Play;

	void UpdatePlayMode(float dt);
	virtual void UpdatePlay(float dt);
	virtual void UpdateStop(float dt);
	virtual void UpdateCutScene(float dt);
	virtual void UpdateChangeScene(float dt);
	virtual void UpdateDebug(float dt);

	void DebugInputUpdate(float dt);
public:
	void Save();
	void Load();

	//virtual void LoadSceneFile() = 0;

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

	InGamePlayer* m_Player;

	Inventory* m_Inventory;
	InventoryUI* m_InventoryUI;

	EventUI* m_PopUpUI;
	GameInfoUI* m_GameInfoUI;



	Tool* m_TestItem;

	bool m_IsLoaded = false;
	SceneRawFile m_SceneFile;
};

