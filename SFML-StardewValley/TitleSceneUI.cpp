#include "pch.h"
#include "TitleSceneUI.h"
#include "TexCoordTable.h"


TitleSceneUI::TitleSceneUI()
{
}

TitleSceneUI::~TitleSceneUI()
{
}

bool TitleSceneUI::Initialize()
{
	m_Logo = new DSprite();
	SetDrawableObj(m_Logo);
	m_Logo->SetPriorityType(DrawPriorityType::Custom, 0);
	m_Logo->SetDebugDraw(false);

	for (int i = 0; i < 4; i++)
	{
		m_Buttons[i] = new DSprite();
		SetDrawableObj(m_Buttons[i]);
		m_Buttons[i]->SetPriorityType(DrawPriorityType::Custom, 1);
		m_Buttons[i]->SetDebugDraw(false);
	}

	return true;
}

void TitleSceneUI::Reset()
{
	const auto& texres = TEXRESTABLE_MGR->GetTileTexRes("Title Screen.png#Logo");

	m_Logo->SetTexture(texres.filepath);
	m_Logo->SetTextureRect(texres.texcoord);
	m_Logo->setLocalPosition({ 0, -80.0f });
	m_Logo->SetOrigin(OriginType::MC);

	const auto& texresbtt0 = TEXRESTABLE_MGR->GetTileTexRes("Title Screen.png#bttNew");
	m_Buttons[0]->SetTexture(texresbtt0.filepath);
	m_Buttons[0]->SetTextureRect(texresbtt0.texcoord);
	m_Buttons[0]->setLocalPosition({ -120, 80.0f });
	m_Buttons[0]->SetOrigin(OriginType::MC);

	const auto& texresbtt1 = TEXRESTABLE_MGR->GetTileTexRes("Title Screen.png#bttLoad");
	m_Buttons[1]->SetTexture(texresbtt1.filepath);
	m_Buttons[1]->SetTextureRect(texresbtt1.texcoord);
	m_Buttons[1]->setLocalPosition({ -40,  80.0f });
	m_Buttons[1]->SetOrigin(OriginType::MC);

	const auto& texresbtt2 = TEXRESTABLE_MGR->GetTileTexRes("Title Screen.png#bttCoop");
	m_Buttons[2]->SetTexture(texresbtt2.filepath);
	m_Buttons[2]->SetTextureRect(texresbtt2.texcoord);
	m_Buttons[2]->setLocalPosition({ 40,80.0f });
	m_Buttons[2]->SetOrigin(OriginType::MC);
	m_Buttons[2]->SetColor(ColorPalette::Gray);

	const auto& texresbtt3 = TEXRESTABLE_MGR->GetTileTexRes("Title Screen.png#bttExit");
	m_Buttons[3]->SetTexture(texresbtt3.filepath);
	m_Buttons[3]->SetTextureRect(texresbtt3.texcoord);
	m_Buttons[3]->setLocalPosition({ 120,  80.0f });
	m_Buttons[3]->SetOrigin(OriginType::MC);

	setScale(3, 3);
	SetAspectRatioPosition(sf::Vector2f(0.5f, 0.5f));

	m_NeedWait = false;
}

void TitleSceneUI::Update(float dt)
{
	m_HasFocus = true;

	if (m_NeedWait)
	{
		m_Timer += dt;
		if (m_Timer > 3.0f)
		{
			Button(m_NowButton);
			return;
		}
	}
	else
	{
		bool resetprev = true;
		for (int i = 0; i < 4; i++)
		{
			if (i == 2)continue;

			if (m_Buttons[i]->GetGlobalBounds().contains(INPUT_MGR->GetMouseViewPos(m_UIViewIndex)))
			{
				resetprev = false;
				if (m_PrevButton != i)
				{
					m_Buttons[i]->setLocalScale({ 1.1f, 1.1f });
					SOUND_MGR->PlaySfx("sounds/Interface/smallSelect.wav");
				}
				m_PrevButton = i;

				if (INPUT_MGR->GetMouseDown(sf::Mouse::Left))
				{
					SOUND_MGR->PlaySfx("sounds/Interface/select.wav");
					m_NowButton = i;
					m_NeedWait = true;
					m_Timer = 0.0f;
					return;
				}
			}
			else
			{
				m_Buttons[i]->setLocalScale({ 1.0f, 1.0f });
			}
		}

		if (resetprev)
			m_PrevButton = -1;
	}
}

void TitleSceneUI::Release()
{
}

void TitleSceneUI::Button(int index)
{
	if (index == 0)
	{
		SCENE_MGR->ChangeScene("InGame::FarmHouseIndoor");
	}
	else if (index == 1)
	{
		SCENE_MGR->ChangeScene("InGame::FarmHouseIndoor");
	}
	else if (index == 3)
	{

	}
}
