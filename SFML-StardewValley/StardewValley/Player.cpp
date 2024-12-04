#include "pch.h"
#include "Player.h"
#include "Tile/TileObjectSystem.h"

Player::Player(const std::string& name)
	:GameObject(name)
{
}

Player::~Player()
{
}

bool Player::Initialize()
{
	AnimationClips();
	animator.SetPlayerAniClip(&temp);

	m_CurrDir = Direction::down;
	m_CurrAction = Action::idle;
	m_CurrEquip = IsVisibleItem::invisibleItem;
	body->setScale({ 2.f, 2.f });
	body->SetPriorityType(DrawPriorityType::Custom, 1);
	return true;

}

void Player::Reset()
{
	animator.Play(&temp["IdleDowninVisible"]);
}

void Player::Update(float dt)
{
	m_PlayerTileIndex = m_TileSystem->GetTileCoordinatedTileIndex(getPosition());
	std::cout << m_PlayerTileIndex.x << "," << m_PlayerTileIndex.y << std::endl;
	animator.Update(dt);
	Staminagauge();

	switch (m_CurrAction)
	{
	case Action::idle:
		UpdateIdle(dt);
		break;
	case Action::move:
		UpdateMove(dt);
		break;
	case Action::interaction:
		UpdateInter(dt);
		break;
	}


	if (direction.x != 0.f || direction.y != 0.f)
	{
		auto min = std::min_element(clipInfos.begin(), clipInfos.end(),
			[this](const ClipInfo& lhs, const ClipInfo& rhs)
			{
				float d1 = Utils::Distance(direction, lhs.point);
				float d2 = Utils::Distance(direction, rhs.point);
				return d1 < d2;
			}
		);
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Right)) {
		isActiveWeapon = !isActiveWeapon;
	}
	if (!isActiveWeapon)
	{
		m_CurrEquip = IsVisibleItem::invisibleItem;
	}
	if (isActiveWeapon)
	{
		m_CurrEquip = IsVisibleItem::visibleItem;
	}

	const auto& clipId = GetAnimationClipIdByDAI();

	if (animator.GetCurrentClipId() != clipId)
	{
		animator.Play(&temp[clipId], true);
	}
}

void Player::UpdateIdle(float dt)
{

	direction.x = INPUT_MGR->GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR->GetAxisRaw(Axis::Vertical);
	float mag = Utils::Magnitude(direction);

	if (mag >= 1.f)
	{
		SetAction(Action::move);
	}
	if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Left)) {
		SetAction(Action::interaction);
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::E)) {
		SetAction(Action::wateringAction);
	}
	if (stamina == 0) {
		SetAction(Action::staminaExhausted);
	}
}

void Player::UpdateMove(float dt)
{
	direction.x = INPUT_MGR->GetAxisRaw(Axis::Horizontal);
	direction.y = INPUT_MGR->GetAxisRaw(Axis::Vertical);

	Utils::Normailize(direction);

	if (direction.x > 0)
		m_CurrDir = Direction::right;
	else if (direction.x < 0)
		m_CurrDir = Direction::left;
	if (direction.y > 0)
		m_CurrDir = Direction::down;
	else if (direction.y < 0)
		m_CurrDir = Direction::up;
	float mag = Utils::Magnitude(direction);

	if (mag == 0)
	{
		SetAction(Action::idle);
	}
	setPosition(getPosition() + direction * speed * dt);
	if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Left)) {
		SetAction(Action::interaction);
	}
}

void Player::UpdateInter(float dt)
{
	if (stamina == 0) {
		SetAction(Action::staminaExhausted);
	}
}



void Player::LateUpdate(float dt)
{
}

void Player::FixedUpdate(float dt)
{
}

void Player::PreRender()
{
}

void Player::PostRender()
{
}

void Player::ImGuiUpdate()
{
}

void Player::Release()
{
}

void Player::AnimationClips()
{
	std::string textureId = "graphics/farmer_base.png";
	body = new DSprite(textureId);
	SetDrawableObj(body);


	int width = 20;
	int height = 31;

	{
		AnimationClip clip;
		clip.id = "IdleSideinVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0 , height, width, height } });
		temp.insert({ "IdleSideinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleDowninVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0, 0, width, height } });
		temp.insert({ "IdleDowninVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleUpinVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, 2 * height, width, height } });
		temp.insert({ "IdleUpinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterSideinVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0 , height, width, height } });
		temp.insert({ "InterSideinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterDowninVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0, 0, width, height } });
		temp.insert({ "InterDowninVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterUpinVisible";
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, 2 * height, width, height } });
		temp.insert({ "InterUpinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveDowninVisible";
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, 0, width, height } });
		}
		temp.insert({ "MoveDowninVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveSideinVisible";
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, { i * width, height, width, height } });
		}
		temp.insert({ "MoveSideinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveUpinVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, { i * width, 2 * height, width, height } });
		}
		temp.insert({ "MoveUpinVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterDownVisible";
		clip.fps = 18;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 3 + 4, width, height } });
		}
		temp.insert({ "InterDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterSideVisible";
		clip.fps = 18;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 4 + 4, width, height } });
		}
		temp.insert({ "InterSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterUpVisible";
		clip.fps = 18;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 5 + 4 , width, height } });
		}
		temp.insert({ "InterUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "AttackDownVisible";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 6, width, height } });
		}
		temp.insert({ "AttackDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "AttackSideVisible";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 7, width, height } });
		}
		temp.insert({ "AttackSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "AttackUpVisible";
		clip.fps = 20;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 7; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 8, width, height } });
		}
		temp.insert({ "AttackUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleDownVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, height * 13, width, height } });

		temp.insert({ "IdleDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleSideVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, height * 14, width, height } });
		temp.insert({ "IdleSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleUpVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, height * 15, width, height } });
		temp.insert({ "IdleUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveDownVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 13, width, height } });
		}
		temp.insert({ "MoveDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveSideVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 14, width, height } });
		}
		temp.insert({ "MoveSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveUpVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 15, width, height } });
		}
		temp.insert({ "MoveUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "WaterDownVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 5; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 10, width, height } });
		}
		temp.insert({ "WaterDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "WaterSideVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 5; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 9, width, height } });
		}
		temp.insert({ "WaterSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "WaterUpVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 5; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 11, width, height } });
		}
		temp.insert({ "WaterUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "ExhaustDownVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 12, width, height } });
		}
		temp.insert({ "ExhaustDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "ExhaustSideVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 12, width, height } });
		}
		temp.insert({ "ExhaustSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "ExhaustUpVisible";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 12, width, height } });
		}
		temp.insert({ "ExhaustUpVisible", clip });
	}


	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, Utils::GetNormal({1.f, -1.f}) });
	clipInfos.push_back({ "IdleUpinVisible", "MoveUpinVisible", false, {0.f, -1.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, Utils::GetNormal({-1.f, -1.f}) });

	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, {1.f, 0.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, {-1.f, 0.f} });

	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, Utils::GetNormal({1.f, 1.f}) });
	clipInfos.push_back({ "IdleDowninVisible", "MoveDowninVisible", false, {0.f, 1.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, Utils::GetNormal({-1.f, 1.f}) });


	animator.SetTarget(body);

	animator.AddEvent("InterUpVisible", 5, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("InterSideVisible", 5, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("InterDownVisible", 5, [&]() {this->m_CurrAction = Action::idle; });

	animator.AddEvent("WaterDownVisible", 4, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("WaterSideVisible", 4, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("WaterUpVisible", 4, [&]() {this->m_CurrAction = Action::idle; });

	animator.AddEvent("ExhaustDownVisible", 5, [&]() {this->m_CurrAction = Action::idle, stamina = 20; });
	animator.AddEvent("ExhaustSideVisible", 5, [&]() {this->m_CurrAction = Action::idle, stamina = 20; });
	animator.AddEvent("ExhaustUpVisible", 5, [&]() {this->m_CurrAction = Action::idle, stamina = 20; });
}

std::string Player::GetAnimationClipIdByDAI()
{
	std::string id;

	switch (m_CurrAction)
	{
	case Player::Action::idle:
		id += "Idle";
		break;
	case Player::Action::move:
		id += "Move";
		break;
	case Player::Action::interaction:
		id += "Inter";
		break;
	case Player::Action::wateringAction:
		id += "Water";
		break;
	case Player::Action::staminaExhausted:
		id += "Exhaust";
		break;
	default:
		break;
	}

	switch (m_CurrDir)
	{
	case Player::Direction::up:
		id += "Up";
		break;
	case Player::Direction::down:
		id += "Down";
		break;
	case Player::Direction::left:
		id += "Side";
		setScale({ -1.f, 1.0f });
		break;
	case Player::Direction::right:
		id += "Side";
		setScale({ 1.f, 1.0f });
		break;
	default:
		break;
	}

	switch (m_CurrEquip)
	{
	case Player::IsVisibleItem::visibleItem:
		id += "Visible";
		break;
	case Player::IsVisibleItem::invisibleItem:
		id += "inVisible";
		break;
	default:
		break;
	}

	return id;
}

float Player::Staminagauge()
{
	if (stamina < 0) {
		value = 0.0f;
	}
	else {
		value = (float)stamina / maxStamina;
	}
	return value;
}

Player::Direction Player::GetDirection()
{
	return m_CurrDir;
}

Player::IsVisibleItem Player::GetIsVisibleItem()
{
	return m_CurrEquip;
}

void Player::GetHoe(Hoe* hoe)
{
	this->hoe = hoe;

}

void Player::SetAction(Action newAction)
{
	Action prevAction = m_CurrAction;
	m_CurrAction = newAction;

	switch (m_CurrAction)
	{
	case Action::idle:

		break;
	case Action::move:
		break;
	case Action::interaction:
		hoe->Use(this);
		stamina--;
		break;
	case Action::wateringAction:
		stamina--;
		break;
	case Action::staminaExhausted:

		break;
	}
}


