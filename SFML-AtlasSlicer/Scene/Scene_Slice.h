#pragma once
#include "SceneBase.h"
class TargetRect;
class TileGrid;

class Scene_Slice :
    public SceneBase
{
public:
	Scene_Slice();
	~Scene_Slice();

	bool Initialize() override;
	void Reset() override;
	void Enter() override;
	void Update(float dt) override;
	void ShowSceneImgui() override;

	void Save(const std::string& id, const std::string& filepath, float rect[]);
	void SaveAsPng(const std::string filepath, const std::string& id, const sf::IntRect& rect, const sf::Image& image);
private:

	SpriteObject* m_Target;
	DSprite* m_TargetSprite;
	TargetRect* m_TargetRect;
	DRectangle* m_TargetRectangle;
	TileGrid* m_Grid;

	char m_FilePathBuff[1000] = {};
	std::string m_CurrentFilePath = "";

	char m_IdBuff[1000] = {};
	std::string m_CurrentId = "";

	bool m_IsSlicingNow=false;
	int m_SliceXCount=1;
	int m_SliceYCount=1;
	float m_Rect[4];
	float m_Color[3] = { 1,1,1 };
	int m_Unit = 10;
	
};

