#pragma once

class SpriteObject;
class Scene_Test:
	public SceneBase
{
public:
	Scene_Test();
	~Scene_Test();

	bool Initialize() override;
	void Reset() override;
	void Enter() override;
	void Update(float dt) override;

	SpriteObject* temp;
	Animator animator;
};

