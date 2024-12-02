#pragma once
#include "AnimationClip.h"

template<typename T>
class ResourceManager :
	public Singleton<ResourceManager<T>>
{
	friend Singleton<ResourceManager<T>>;	//부모생성자에서 생성을 못해주기 때문에 프랜드 선언

public:
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(ResourceManager<T>&&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
protected:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	static std::unordered_map<std::string, T*> m_Resources;
	std::map<std::string, std::tuple<AnimationClip*, bool>> mapAnimationClip;

public:
	bool LoadByFilepath(const std::string& filepath)
	{
		if (m_Resources.find(filepath) == m_Resources.end())
		{
			T* newRes = GetByFilepath(filepath);
			return newRes;
		}
		return false;
	}

	bool UnloadByFilepath(const std::string& filepath)
	{
		if (!IsValidPath(filepath))
		{
			return false;
		}

		auto it = m_Resources.find(filepath);
		if (it == m_Resources.end())
		{
			return false;
		}

		delete it->second;
		m_Resources.erase(it);
		return true;
	}

	void UnloadAll()
	{
		for (const auto& pair : m_Resources)
		{
			delete pair.second;
		}
		m_Resources.clear();
	}

	T* Get(const std::string& id)
	{
		auto it = m_Resources.find(id);
		if (it == m_Resources.end())
		{
			std::cout << "RESOURCE Get::\"" << id << "\"		...	";
			std::cout << "Fail\n";
			return nullptr;
		}
		else
		{
			return it->second;
		}
	}


	T* GetByFilepath(const std::string& filepath) //실패시 nullptr반환
	{
		if (!IsValidPath(filepath))
		{
			return nullptr;
		}

		auto it = m_Resources.find(filepath);
		if (it == m_Resources.end())
		{
			T* newRes = new T();
			std::cout << "RESOURCE LOADING::\"" << filepath << "\"		...	";
			if (newRes->loadFromFile(filepath))
			{
				std::cout << "Success\n";
				m_Resources[filepath] = newRes;
				return newRes;
			}
			else
			{
				std::cout << "Fail\n";
				delete newRes;
				return nullptr;
			}
		}
		else
		{
			return it->second;
		}
	}

	T* GetByFilepath(const std::string& filepath, const std::string& id) //실패시 nullptr반환
	{
		if (!IsValidPath(filepath))
		{
			return nullptr;
		}

		auto it = m_Resources.find(filepath);
		if (it == m_Resources.end())
		{
			T* newRes = new T();
			std::cout << "RESOURCE LOADING::\"" << filepath << "\"		...	";
			if (newRes->loadFromFile(filepath))
			{
				std::cout << "Success\n";
				m_Resources[id] = newRes;
				return newRes;
			}
			else
			{
				std::cout << "Fail\n";
				delete newRes;
				return nullptr;
			}
		}
		else
		{
			return it->second;
		}
	}


	bool IsValidPath(const std::string& filepath)
	{
		if (filepath.compare("") == 0)
		{
			std::cout << "RESOURCE Filepath::\"" << filepath << "\"is not vaild\n";
			return false;
		}
		return true;
	}

	T* GetAnimationClip(const std::string& id)
	{
		auto it = mapAnimationClip.find(id);
		if (it != mapAnimationClip.end())
		{
			return get<0>(it->second);
		}
		return nullptr;
	}
};

template<typename T>
std::unordered_map<std::string, T*>  ResourceManager<T>::m_Resources = std::unordered_map<std::string, T*>();

#define TEXTURE_MGR (ResourceManager<sf::Texture>::GetInstance())
#define FONT_MGR (ResourceManager<sf::Font>::GetInstance())
#define SOUNDBUFFER_MGR (ResourceManager<sf::SoundBuffer>::GetInstance())
#define ANI_CLIP_MGR (ResourceManager<AnimationClip>::GetInstance())
