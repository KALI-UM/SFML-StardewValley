#pragma once

class TileObjectSystem;
class TileModel;
class TileView;
class TileGrid;
class TileObject;

class Player;

enum class InGamePlayMode
{
	Play,
	Stop,
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

	void SetPlayMode(InGamePlayMode mode);
	InGamePlayMode GetPlayMode() const { return m_CurrPlayMode; }
protected:
	InGamePlayMode m_CurrPlayMode = InGamePlayMode::Play;
	
	void UpdatePlayMode(float dt);
	virtual void UpdatePlay(float dt);
	virtual void UpdateStop(float dt);
	virtual void UpdateDebug(float dt);
	
	void DebugInputUpdate(float dt);
	
private:
	

protected:
	TileObjectSystem*	m_TileObjectSystem;
	TileModel*			m_TileModel;
	TileView*			m_TileView;
	TileGrid*			m_TileGrid;

	std::string			m_TerrainFilepath;
	TileObject*			m_Back;
	TileObject*			m_Front;

	Player*				m_Player;
	
};

