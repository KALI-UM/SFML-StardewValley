#pragma once
#include "GameObject.h"
class Player;

class PlayerStatusUi : public GameObject
{
protected:

	DSprite* StaminaUi;
	DRectangle* Staminabar;
	Player* player;
	DSprite* dashboardInfo;
	DSprite* ClockHands;

	float count = 0;

public:
	PlayerStatusUi();
	~PlayerStatusUi();

	bool Initialize();
	void Reset();
	void Update(float dt);

	void UpdateIdle(float dt);
	void UpdateMove(float dt);
	void UpdateInter(float dt);


	void LateUpdate(float dt);
	void FixedUpdate(float dt);
	void PreRender();
	void PostRender();
	void ImGuiUpdate();
	void Release();
};

