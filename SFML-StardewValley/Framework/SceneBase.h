#pragma once

class GameObject;
class UIObject;
class SceneBase
{
public:
	SceneBase(const std::string& name, unsigned int layercnt = 2, int viewcnt = 1);
	//SceneBase(const SceneBase& other);
	//SceneBase(SceneBase&& other);
	virtual ~SceneBase();

	bool INITIALIZE();
	void RESET();
	void ENTER();
	void UPDATE(float dt);
	void LATEUPDATE(float dt);
	void FIXEDUPDATE(float dt);
	void IMGUIUPDATE();
	void PRERENDER();
	void POSTRENDER();
	void EXIT();
	void RELEASE();

	virtual void ShowSceneImgui() {};

	std::string GetName()const;

	typedef std::pair<int, GameObject*> GameObjectInfo;


	void RemoveGameObject(GameObjectInfo gobj);
	void RemoveGameObject(int layerIndex, GameObject* gobj);

	struct ViewInfo
	{
		int viewIndex;
		bool needPriority;
	};

	const unsigned int				m_ViewCnt;
	std::vector<ViewInfo>			m_ViewInfo;
	const unsigned int				m_UILayerIndex;
	const unsigned int				m_UIViewIndex;
protected:
	virtual bool Initialize();
	virtual void Reset();
	virtual void Enter();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void FixedUpdate(float dt);
	virtual void ImGuiUpdate();
	virtual void PreRender();
	virtual void PostRender();
	virtual void Exit();
	virtual void Release();


public:
	template <typename T>
	T* AddGameObject(int layerIndex, T* gameObject)
	{
		m_WantsToAdd.push({ layerIndex , gameObject });
		return gameObject;
	}

	template <typename T>
	T* AddUIGameObject(int layerIndex, T* gameObject)
	{
		UIObject* ui = dynamic_cast<UIObject*>(gameObject);
		if (ui)
		{
			m_WantsToAddUI.push(ui);
			ui->m_UIViewIndex = m_UIViewIndex;
		}
		return AddGameObject(layerIndex, gameObject);
	}

	template <typename T>
	T* FindGameObject(int layerIndex, const std::string& name)
	{
		if (name == "")
			return nullptr;
		auto layerit = GetGameObjectsLayerIter(layerIndex);
		for (auto& currgobj : layerit)
		{
			if (currgobj->m_Name == name)
				return dynamic_cast<T*>(currgobj);
		}
		std::cout << "FindGameObject(" << layerIndex << "," << name << ") Fail" << std::endl;
		return nullptr;
	}


protected:
	void SetLayerViewIndex(int layerIndex, int viewIndex);
	void SetViewNeedPriority(int viewIndex, bool needPriority);
	void RenderViewToRenderTexture(int viewindex, sf::RenderTexture& texture);

private:
	const std::string							m_Name;
	struct LayerInfo
	{
		int										viewIndex = 0;
		std::vector<GameObject*>				gameObjects;
	};


	std::list<LayerInfo>						m_GameObjects;
	std::queue<GameObjectInfo>					m_WantsToAdd;
	std::queue<GameObjectInfo>					m_WantsToRemove;

	std::vector<GameObject*>& GetGameObjectsLayerIter(int index);
	std::vector<std::list<LayerInfo>::iterator> m_LayerIndex;

public:
	bool GetUIHasFocus() const { return m_UIHasFocus; }
	sf::Vector2f GetMouseUIViewPos() const;

protected:
	virtual void UIUpdate(float dt);
	std::list<UIObject*>						m_UIGameObjects;
	std::queue<UIObject*>						m_WantsToAddUI;
	std::queue<UIObject*>						m_WantsToRemoveUI;
	bool										m_UIHasFocus = false;
private:
	void PushToDrawQue();
	void PushLayerToDrawQueDebugQue(int layerIndex);
	void PushLayerToDrawQueNoCulling(int layerIndex);
	void PushLayerToDrawQue(int layerIndex);

	void RegisterGameObject();
	void RemoveGameObject();

	void RegisterUIObject();
	void RemoveUIObject();
};

#define UI_HASFOCUS (SCENE_MGR->GetCurrentScene()->GetUIHasFocus())