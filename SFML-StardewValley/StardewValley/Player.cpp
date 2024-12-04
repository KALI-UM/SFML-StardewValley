#include "pch.h"
#include "Player.h"
#include "Item/Tool.h"
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


	setPosition(150, 150);

	return true;

}

void Player::Reset()
{
	animator.Play("datatables/AnimationDataTable/Player-IdleDowninVisible.csv");
}

void Player::Update(float dt)
{
	m_PlayerTileIndex = m_TileSystem->GetTileCoordinatedTileIndex(GAME_MGR->GetScreenToViewPos(0, GAME_MGR->GetViewToScreenPos(1, getPosition())));
	//std::cout << m_PlayerTileIndex.x << "," << m_PlayerTileIndex.y << std::endl;

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
		std::cout << clipId << std::endl;
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
	/*if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Left)) {
		SetAction(Action::interaction);
	}*/
	if (INPUT_MGR->GetMouseDown(sf::Mouse::Button::Left)) {
		SetAction(Action::Attack);
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

	sf::Vector2f nextpos = getPosition() + direction * speed * dt;

	CellIndex nextTileIndex = m_TileSystem->GetTileCoordinatedTileIndex(GAME_MGR->GetScreenToViewPos(0, GAME_MGR->GetViewToScreenPos(1, nextpos)));
	if (m_TileSystem->IsPossibleToPass(nextTileIndex))
	{
		std::cout << nextTileIndex.x << "," << nextTileIndex.y << std::endl;
		setPosition(nextpos);
	}
	else
	{
		int k = 0;
	}

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

	body = new DSprite();
	SetDrawableObj(body);

	rapidcsv::Document doc("datatables/AnimationDataTable/PlayerAnimationFileList.csv", rapidcsv::LabelParams(-1, -1));
	std::vector<std::string> animationfilepath = doc.GetColumn<std::string>(0);
	AnimationClip clip;
	for (auto& curr : animationfilepath)
	{
		ANI_CLIP_MGR->LoadByFilepath(curr, clip.GetIdFromFilepath(curr));
		clip.loadFromFile(curr);
		temp.insert({ clip.GetIdFromFilepath(curr),  clip });
	}
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

	animator.AddEvent("AttackDownVisible", 5, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("AttackSideVisible", 5, [&]() {this->m_CurrAction = Action::idle; });
	animator.AddEvent("AttackUpVisible", 5, [&]() {this->m_CurrAction = Action::idle; });
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
	case Player::Action::Attack:
		id += "Attack";
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

void Player::GetTool(Tool* tool)
{
	this->tool = tool;
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
		tool->Use(this);
		stamina--;
		break;
	case Action::wateringAction:
		stamina--;
		break;
	case Action::staminaExhausted:

		break;
	case Player::Action::Attack:
		tool->Use(this);
		stamina--;
		break;
	}

}

void Player::GetItemType(ItemType type)
{
	itemtype = type;
}



