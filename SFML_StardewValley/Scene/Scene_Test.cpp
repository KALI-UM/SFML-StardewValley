#include "pch.h"
#include "Scene_Test.h"
#include "Player.h"
#include "Animator.h"

Scene_Test::Scene_Test()
	:SceneBase("Test", 1, 1)
{
}

Scene_Test::~Scene_Test()
{
}

bool Scene_Test::Initialize()
{
	temp = AddGameObject(0, new Player());

	return false;
}

void Scene_Test::Reset()
{

}

void Scene_Test::Enter()
{
}

void Scene_Test::Update(float dt)
{
	animator.Update(dt);
}
