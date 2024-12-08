#include "pch.h"
#include "Scene_Tiltle.h"
#include "TitleSceneUI.h"

Scene_Tiltle::Scene_Tiltle()
	:SceneBase("Title", 2, 2)
{
}

Scene_Tiltle::~Scene_Tiltle()
{
}

bool Scene_Tiltle::Initialize()
{
	m_Panorama = AddGameObject(0, new SpriteObject());
	m_Panorama->SetTexture("graphics/PC Computer - Stardew Valley - Panorama.png");
	m_UI = AddUIGameObject(m_UILayerIndex, new TitleSceneUI());

	return false;
}

void Scene_Tiltle::Enter()
{

	//SOUND_MGR->PlayBgm
}

void Scene_Tiltle::Update(float dt)
{
}

void Scene_Tiltle::OnWindowResize()
{
	for (int layer = 0; layer < m_UILayerIndex; layer++)
		GAME_MGR->SetViewZoom(layer, 0.25f);

	GAME_MGR->SetViewCenter(0, sf::Vector2f(m_Panorama->GetSprite()->GetTextureSize().x / 2, m_Panorama->GetSprite()->GetTextureSize().y / 2));
}
