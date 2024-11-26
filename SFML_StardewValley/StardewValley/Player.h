#pragma once

class Player : public GameObject
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
		bool flipX = false;
		sf::Vector2f point;
	};

protected:
	DSprite* body;
	DSprite* item;

	sf::Vector2f direction;
	float speed = 500.f;

	std::map<std::string, AnimationClip> temp;
	std::vector<ClipInfo> clipInfos;
	ClipInfo* currentClipInfo = nullptr;

	Animator animator;
public:
	Player();
	~Player();

	bool Initialize();
	void Reset();
	void Update(float dt);
	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void PreRender();
	void PostRender();
	void ImGuiUpdate();
	void Release();
};

