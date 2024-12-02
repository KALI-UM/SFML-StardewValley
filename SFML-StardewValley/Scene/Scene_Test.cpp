#include "pch.h"
#include "Scene_Test.h"
#include "Player.h"
#include "Animator.h"
#include "PlayerStatusUi.h"
#include "Item/Hoe.h"

Scene_Test::Scene_Test()
	:SceneBase("Test", 1, 1)
{
}

Scene_Test::~Scene_Test()
{
}

bool Scene_Test::Initialize()
{
	SetLayerViewIndex(0, 0);
	temp = AddGameObject(0, new Player("Player"));
	statusUi = AddGameObject(m_UILayerIndex, new PlayerStatusUi());
	hoe  = AddGameObject(0, new Hoe());
	return false;
}

void Scene_Test::Reset()
{
	temp->GetHoe(hoe);
	hoe->GetPlayer(temp);
}

void Scene_Test::Enter()
{
	
	
}

void Scene_Test::Update(float dt)
{


	if (temp != nullptr)
	{
		GAME_MGR->SetViewCenter(0, temp->getPosition());
	}
	statusUi->setPosition(sf::Vector2f(GAME_MGR->GetWindow()->getSize().x-100, GAME_MGR->GetWindow()->getSize().y - 100));


}
