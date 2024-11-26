#include "pch.h"
#include "Scene_Test.h"
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
	temp=AddGameObject(0, new SpriteObject("background/SimCity_2000_screen.png"));

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "!!" << std::endl;
		}
	);

	animator.AddEvent("Idle", 0,
		[]()
		{
			std::cout << "??" << std::endl;
		}
	);

	return false;
}

void Scene_Test::Reset()
{
	animator.SetTarget(dynamic_cast<sf::Sprite*>(temp->GetDrawableObj(0)->GetDrawable()));
	animator.Play("datatables/idle.csv");
}

void Scene_Test::Enter()
{
}

void Scene_Test::Update(float dt)
{
	animator.Update(dt);
}
