#include "pch.h"
#include "Scene_InGameTest.h"
#include "Scene_InGame.h"
#include "Scene_FarmHouseIndoor.h"
#include "Scene_FarmLand.h"
#include "Scene_TileEditor.h"


GameManager::GameManager()
	:m_MainWindow(nullptr)
{
}

bool GameManager::Initialize(sf::RenderWindow* window)
{
	m_MainWindow = window;
	bool success = true;
	success &= DATATABLE_MGR->Initialize();
	success &= SCENE_MGR->Initialize();

	Scene_FarmLand* farmland = new Scene_FarmLand();
	SCENE_MGR->PushScene(farmland);

	Scene_FarmHouseIndoor* indoor = new Scene_FarmHouseIndoor();
	SCENE_MGR->PushScene(indoor);

	Scene_TileEditor* editor = new Scene_TileEditor();
	SCENE_MGR->PushScene(editor);


	SCENE_MGR->SetCurrentScene(indoor->GetName());
	indoor->RESET();
	indoor->ENTER();
	return success;
}

void GameManager::Release()
{
	//SCENE_MGR->Release();
	DATATABLE_MGR->Release();
}

void GameManager::UpdateEvent(const sf::Event& ev)
{
	//기본적으로 기본 0번뷰와 ui뷰만 수정하려고 했는데 다해버린다
	if (ev.type == sf::Event::Resized)
	{
		for (int i = 0; i < m_Views.size(); i++)
		{
			SetViewSize(i, sf::FloatRect(0, 0, (float)ev.size.width, (float)ev.size.height));
			//SetViewSize(SCENE_MGR->GetCurrentScene()->m_UIViewIndex, sf::FloatRect(0, 0, (float)ev.size.width, (float)ev.size.height));
		}
	}
}

void GameManager::Update(float dt)
{
	SCENE_MGR->Update(dt);
	SCENE_MGR->PreRender();

	if (INPUT_MGR->GetKeyDown(sf::Keyboard::F1))
	{
		std::cout << "Editor Scene\n";
		SCENE_MGR->ChangeScene("TileEditor");
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::F2))
	{
		//std::cout << "Test Scene\n";
		//SCENE_MGR->ChangeScene("InGameTest");
	}

	if (INPUT_MGR->GetKeyDown(sf::Keyboard::F5))
	{
		std::cout << "Play Mode - Debug\n";
		m_GameMode = GameMode::Debug;
	}
	if (INPUT_MGR->GetKeyDown(sf::Keyboard::F6))
	{
		std::cout << "Play Mode - Normal\n";
		m_GameMode = GameMode::Normal;
	}
}

void GameManager::Render()
{
	const sf::View& defaultView = m_MainWindow->getView();
	for (int i = 0; i < m_Views.size(); i++)
	{
		m_MainWindow->setView(m_Views[i].view);

		if (m_Views[i].needPriority)
		{
			auto& currDrawQue = m_Views[i].drawQue_PQ;
			while (!currDrawQue.empty())
			{
				auto& curr = currDrawQue.top();
				m_MainWindow->draw(*curr->GetDrawable(), curr->getParentTransform());
				currDrawQue.pop();
			}
		}
		else
		{
			auto& currDrawQue = m_Views[i].drawQue_Q;
			while (!currDrawQue.empty())
			{
				auto& curr = currDrawQue.front();
				m_MainWindow->draw(*curr->GetDrawable(), curr->getParentTransform());
				currDrawQue.pop();
			}
		}


#ifdef _DEBUG
		m_MainWindow->setView(m_DebugViews[i].view);
		auto& currDebugDrawQue = m_DebugViews[i].drawQue;
		while (!currDebugDrawQue.empty())
		{
			m_MainWindow->draw((sf::Drawable&)*currDebugDrawQue.front());
			currDebugDrawQue.pop();
		}
#endif // _DEBUG
	}

	m_MainWindow->setView(defaultView);
	SCENE_MGR->PostRender();
}

void GameManager::LateUpdate(float dt)
{
	SCENE_MGR->LateUpdate(dt);
}

void GameManager::FixedUpdate(float dt)
{
	SCENE_MGR->FixedUpdate(dt);
}

void GameManager::ImGuiUpdate()
{
	SCENE_MGR->ImGuiUpdate();
}

sf::RenderWindow* GameManager::GetWindow()
{
	return m_MainWindow;
}

sf::View* GameManager::GetView(int index)
{
	return &m_Views[index].view;
}

const sf::FloatRect& GameManager::GetViewRect(int index)
{
	return m_Views[index].viewRect;
}

void GameManager::ResizeViews(unsigned int cnt)
{
	m_Views.resize(cnt);
#ifdef _DEBUG
	m_DebugViews.resize(cnt);
#endif // _DEBUG

	for (int i = 0; i < m_Views.size(); i++)
	{
		GAME_MGR->SetViewSize(i, { 0,0,(float)GAME_MGR->GetWindow()->getSize().x, (float)GAME_MGR->GetWindow()->getSize().y });
	}
}

void GameManager::SetViewNeedPriority(int index, bool need)
{
	m_Views[index].needPriority = need;
}

int GameManager::GetViewCount()const
{
	return (int)m_Views.size();
}

sf::Vector2f GameManager::GetScreenToViewPos(int index, const sf::Vector2i& screenPos)
{
	return m_MainWindow->mapPixelToCoords(screenPos, m_Views[index].view);
}

sf::Vector2i GameManager::GetViewToScreenPos(int index, const sf::Vector2f& viewPos)
{
	return m_MainWindow->mapCoordsToPixel(viewPos, m_Views[index].view);
}

void GameManager::SetViewSize(int index, const sf::FloatRect& rect)
{
	m_Views[index].view.reset(rect);
#ifdef _DEBUG
	m_DebugViews[index].view.reset(rect);
#endif // _DEBUG
}

void GameManager::SetViewZoom(int index, float zoom)
{
	m_Views[index].view.zoom(zoom);
#ifdef _DEBUG
	m_DebugViews[index].view.zoom(zoom);
#endif // _DEBUG
}

void GameManager::SetViewportSize(int index, const sf::FloatRect& rect)
{
	m_Views[index].view.setViewport(rect);
#ifdef _DEBUG
	m_DebugViews[index].view.setViewport(rect);
#endif // _DEBUG
}

void GameManager::SetViewCenter(int index, const sf::Vector2f& pos)
{
	m_Views[index].view.setCenter(pos);
#ifdef _DEBUG
	m_DebugViews[index].view.setCenter(pos);
#endif // _DEBUG
}

void GameManager::MoveView(int index, const sf::Vector2f& offset)
{
	m_Views[index].view.move(offset);
#ifdef _DEBUG
	m_DebugViews[index].view.move(offset);
#endif // _DEBUG
}

void GameManager::SetViewRotation(int index, float rot)
{
	m_Views[index].view.setRotation(rot);
#ifdef _DEBUG
	m_DebugViews[index].view.setRotation(rot);
#endif // _DEBUG
}

void GameManager::RotateView(int index, float rot)
{
	m_Views[index].view.rotate(rot);
#ifdef _DEBUG
	m_DebugViews[index].view.rotate(rot);
#endif // _DEBUG
}

void GameManager::UpdateViewRect()
{
	for (auto& view : m_Views)
	{
		view.viewRect = sf::FloatRect(view.view.getCenter() - view.view.getSize() / 2.0f, view.view.getSize());
	}
}

void GameManager::PushDrawableObject_PQ(int viewindex, DrawableObject* dobj)
{
	m_Views[viewindex].drawQue_PQ.push(dobj);
}

void GameManager::PushDrawableObject_Q(int viewindex, DrawableObject* dobj)
{
	m_Views[viewindex].drawQue_Q.push(dobj);
}

void GameManager::PushDebugDrawObject(int viewindex, DebugInfo* dobj)
{
	m_DebugViews[viewindex].drawQue.push(dobj);
}

void GameManager::RenderViewToRenderTexture(int viewindex, sf::RenderTexture& texture)
{
	texture.clear(sf::Color::Transparent);

	if (m_Views[viewindex].needPriority)
	{
		auto& currDrawQue = m_Views[viewindex].drawQue_PQ;
		while (!currDrawQue.empty())
		{
			auto& curr = currDrawQue.top();
			texture.draw(*curr->GetDrawable(), curr->getParentTransform());
			currDrawQue.pop();
		}
	}
	else
	{
		auto& currDrawQue = m_Views[viewindex].drawQue_Q;
		while (!currDrawQue.empty())
		{
			auto& curr = currDrawQue.front();
			texture.draw(*curr->GetDrawable(), curr->getParentTransform());
			currDrawQue.pop();
		}
	}
}

const GameMode& GameManager::GetGameMode() const
{
	return m_GameMode;
}

bool PriorityComp::operator()(DrawableObject*& lhs, DrawableObject*& rhs)
{
	return DrawableObject::PriorityCompare(lhs, rhs);
}
