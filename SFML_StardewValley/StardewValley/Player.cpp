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
	std::string textureId = "graphics/farmer_base.png";
    body = new DSprite(textureId);
    SetDrawableObj(body);


	int width = 20;
	int height = 31;

	{
		AnimationClip clip;
		clip.id = "IdleSide";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0 , height, width, height } });
		temp.insert({ "IdleSide", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleDown";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, { 0, 0, width, height } });
		temp.insert({ "IdleDown", clip });
	}
	{
		AnimationClip clip;
		clip.id = "IdleUp";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Single;
		clip.frames.push_back({ textureId, {0, height, width, height } });
		temp.insert({ "IdleUp", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveSide";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int j = 0; j < 6; ++j)
		{

			clip.frames.push_back({ textureId, { j * width, height, width, height } });

		}
		temp.insert({ "MoveSide", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveUp";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int i = 0; i < 8; ++i)
		{
			clip.frames.push_back({ textureId, { i * width, 2 * height, width, height } });
		}
		temp.insert({ "MoveUp", clip });
	}
	{
		AnimationClip clip;
		clip.id = "MoveDown";
		clip.fps = 10;
		clip.loopType = AnimationLoopTypes::Loop;
		for (int j = 0; j < 8; ++j)
		{
			clip.frames.push_back({ textureId, {j * width, 0, width, height } });
		}
		temp.insert({ "MoveDown", clip });
	}

	clipInfos.push_back({ "IdleSide", "MoveSide", false, Utils::GetNormal({1.f, -1.f}) });
	clipInfos.push_back({ "IdleUp", "MoveUp", false, {0.f, -1.f} });
	clipInfos.push_back({ "IdleSide", "MoveSide", true, Utils::GetNormal({-1.f, -1.f}) });

	clipInfos.push_back({ "IdleSide", "MoveSide", false, {1.f, 0.f} });
	clipInfos.push_back({ "IdleSide", "MoveSide", true, {-1.f, 0.f} });

	clipInfos.push_back({ "IdleSide", "MoveSide", false, Utils::GetNormal({1.f, 1.f}) });
	clipInfos.push_back({ "IdleDown", "MoveDown", false, {0.f, 1.f} });
	clipInfos.push_back({ "IdleSide", "MoveSide", true, Utils::GetNormal({-1.f, 1.f}) });


	animator.SetTarget(dynamic_cast<sf::Sprite*>(body->GetDrawable()));
	return true;
}

void Player::Reset()
{
	animator.Play(&temp["IdleDown"]);
	currentClipInfo = &clipInfos[6];	
}

void Player::Update(float dt)
{
	animator.Update(dt);

	direction.x = INPUT_MGR->GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR->GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normailize(direction);
	}
	setPosition(body->getPosition() + direction * speed * dt);

	//if (direction.x != 0.f || direction.y != 0.f)
	//{
	//	auto min = std::min_element(clipInfos.begin(), clipInfos.end(),
	//		[this](const ClipInfo& lhs, const ClipInfo& rhs)
	//		{
	//			
	//		}
	//	);
	//	currentClipInfo = &(*min);
	//}
	//if (currentClipInfo->flipX)
	//{
	//	SetScale({ -1.f, 1.0f });
	//}
	//else
	//{
	//	SetScale({ 1.f, 1.0f });
	//}

	const auto& clipId = (direction.x != 0.f || direction.y != 0.f) ? currentClipInfo->move : currentClipInfo->idle;

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
