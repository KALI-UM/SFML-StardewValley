#include "pch.h"
#include "Framework.h"
#include <windows.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")


void Framework::Initialize(int width, int height, const std::string& name)
{
	m_MainWindow.create(sf::VideoMode(width, height), name, sf::Style::Default);

	//sf::Cursor cursor;
	//// ���� ����� ���� Ŀ���� �ִٸ� ����
	//cursor.loadFromSystem(sf::Cursor::Arrow);
	//m_MainWindow.setMouseCursor(cursor); // Ŀ���� �ý��� �⺻���� ���� ��, ����� �õ�
	//m_MainWindow.setMouseCursorVisible(false);

	HWND hwnd = m_MainWindow.getSystemHandle();

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style |= WS_OVERLAPPEDWINDOW;
	SetWindowLong(hwnd, GWL_STYLE, style);
	//HMENU hMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU1));
	//SetMenu(hwnd, hMenu);
	//DrawMenuBar(hwnd);
	//SetWindowTheme(hwnd, L" ", L" ");

	

	Utils::Initialize();

	SOUND_MGR->Initialize();
	INPUT_MGR->Initialize();
	GAME_MGR->Initialize(&m_MainWindow);
	ImGuiManager::Init(&m_MainWindow);

	SOUND_MGR->SetGlobalVolume(80);
}

void Framework::Do()
{
	m_MainWindow.setFramerateLimit(60);
	sf::Time dt = clock.restart();

	while (m_MainWindow.isOpen())
	{
		m_RealDeltaTime = m_DeltaTime = dt.asSeconds();
		m_DeltaTime *= m_TimeScale;
		m_RealTime += m_RealDeltaTime;
		m_Time += m_DeltaTime;

		// �̺�Ʈ ����
		INPUT_MGR->Clear();
		sf::Event event;

		while (m_MainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_MainWindow.close();
			ImGuiManager::PollEvent(event);
			INPUT_MGR->UpdateEvent(event);
			GAME_MGR->UpdateEvent(event);

			if (event.type == sf::Event::GainedFocus) {
				// ��Ŀ�� ����
				//m_MainWindow.setMouseCursorVisible(false);
				ImGuiManager::SetImGuiWindowHasFocus(true);
			}
			else if (event.type == sf::Event::LostFocus) {
				// ��Ŀ�� ����
				//m_MainWindow.setMouseCursorVisible(true); // �ʿ�� �� �� ����
				ImGuiManager::SetImGuiWindowHasFocus(false);
			}
		}

		// ������Ʈ
		if (m_RealTime - m_FixedTimePrev > m_FixedTimeStamp)
		{
			m_FixedTimePrev = m_RealTime;
			GAME_MGR->FixedUpdate(m_RealDeltaTime);
		}

		INPUT_MGR->Update(m_RealDeltaTime);
		SOUND_MGR->Update(m_RealDeltaTime);
		GAME_MGR->Update(m_DeltaTime);

		GAME_MGR->LateUpdate(m_DeltaTime);

		// ��ο�
		m_MainWindow.clear(m_BackColor);

		//std::cout << 1.0f / m_RealDeltaTime << std::endl;

		GAME_MGR->Render();

		{ // ImGui Layer
			ImGuiManager::Begin(dt);
			GAME_MGR->ImGuiUpdate();
			ImGuiManager::End();
		}

		m_MainWindow.display();

		dt = clock.restart();
	}
}

void Framework::Release()
{
	GAME_MGR->Release();
	SOUND_MGR->Release();
	ImGuiManager::Release();
}

