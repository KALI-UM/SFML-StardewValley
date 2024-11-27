#include "pch.h"
#include "Player.h"


Player::Player()
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
	return true;

}

void Player::Reset()
{
	animator.Play(&temp["IdleDowninVisible"]);
	
}

void Player::Update(float dt)
{
	animator.Update(dt);

	direction.x = INPUT_MGR->GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR->GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag >= 1.f)
	{
		Utils::Normailize(direction);
		m_CurrAction = Action::move;
		std::cout << "(x : " << direction.x << ", y : " << direction.y << ")" << std::endl;

		if (direction.x > 0)
			m_CurrDir = Direction::right;
		else if(direction.x < 0)
			m_CurrDir = Direction::left;

		if (direction.y > 0)
			m_CurrDir = Direction::down;
		else if (direction.y < 0)
			m_CurrDir = Direction::up;
	}
	else if (mag == 0)
	{
		m_CurrAction = Action::idle;
	}
	setPosition(getPosition() + direction * speed * dt);
	if (direction.x != 0.f || direction.y != 0.f)
	{
		auto min = std::min_element(clipInfos.begin(), clipInfos.end(),
			[this](const ClipInfo& lhs, const ClipInfo& rhs)
			{
				float d1 = Utils::Distance(direction, lhs.point);
				float d2 = Utils::Distance(direction, rhs.point);
				//if (d1 < d2) {
				//	m_CurrDir = Direction::down;
				//}
				//if (d1 > d2) {
				//	m_CurrDir = Direction::up;
				//}
				return d1 < d2;
			}
		);
		currentClipInfo = &(*min);
	}

	if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Left)) {
		m_CurrAction = Action::interaction;
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
		animator.Play(&temp[clipId]);
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
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 3, width, height } });
		}
		temp.insert({ "InterDownVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterSideVisible";
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 4, width, height } });
		}
		temp.insert({ "InterSideVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "InterUpVisible";
		clip.fps = 8;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 6; ++i)
		{
			clip.frames.push_back({ textureId, {i * width, height * 5, width, height } });
		}
		temp.insert({ "InterUpVisible", clip });
	}
	{
		AnimationClip clip;
		clip.id = "AttackDownVisible";
		clip.fps = 10;
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
		clip.fps = 10;
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
		clip.fps = 10;
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

	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, Utils::GetNormal({1.f, -1.f}) });
	clipInfos.push_back({ "IdleUpinVisible", "MoveUpinVisible", false, {0.f, -1.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, Utils::GetNormal({-1.f, -1.f}) });

	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, {1.f, 0.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, {-1.f, 0.f} });

	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", false, Utils::GetNormal({1.f, 1.f}) });
	clipInfos.push_back({ "IdleDowninVisible", "MoveDowninVisible", false, {0.f, 1.f} });
	clipInfos.push_back({ "IdleSideinVisible", "MoveSideinVisible", true, Utils::GetNormal({-1.f, 1.f}) });


	animator.SetTarget(body);
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



