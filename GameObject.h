#pragma once
#include <list>
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"

class Component;
class GameObject;

class GameObjectManager
{
public:
	GameObjectManager()
	{
		number_of_objects = 0;
	};
	GameObject* AddGameObject(Vector3 position, Quaternion quaternion, Vector3 scale);
	size_t& NumberOfGameObjects()
	{
		return number_of_objects;
	}
	std::list<GameObject>* ListOfGameObjects()
	{
		return &list_of_objects;
	}
	void SetCamera(GameObject* camera);
	GameObject* GetCamera()
	{
		return camera;
	}
	GameObject* GetbyIndex(int index)
	{
		std::list<GameObject>::iterator it = list_of_objects.begin();
		std::advance(it, index);
		return &(*it);
	}
	~GameObjectManager() {};
public:
	static GameObjectManager* get()
	{
		return manager;
	};
	static void create()
	{
		if (GameObjectManager::manager) return;
		GameObjectManager::manager = new GameObjectManager();
	};
	static void release()
	{
		if (!GameObjectManager::manager) return;
		delete GameObjectManager::manager;
	};
private:
	static GameObjectManager* manager;
	GameObject* camera;
	std::list<GameObject> list_of_objects;
	size_t number_of_objects;
};

inline GameObjectManager* GameObjectManager::manager = nullptr;

const int MAX_ENTITIES = 32;

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	GameObject* parent;
	Vector3 localPosition;
	Quaternion localRotation;
};

class Component
{
public:
	GameObject* gameobject;
	bool enabled = true;
public:
	virtual void Update() {};

	void Instantiate(Vector3 position, Quaternion rotation, Vector3 scale)
	{
		GameObjectManager::get()->AddGameObject(position, rotation, scale);
	}

	~Component() {};
};

class GameObject
{
public:
	bool enabled;
	Transform transform;
	int componentsNumber = 0;
	Matrix4x4 m_world;
public:
	GameObject()
	{
		enabled = true;
		transform.position = Vector3();
		transform.rotation = Quaternion();
		transform.scale = Vector3(1, 1, 1);
	};

	void processWorldMatrix()
	{
		Matrix4x4 temp;

		m_world.setIdentity();

		temp.setIdentity();
		temp.SetScale(transform.scale);
		m_world *= temp;

		temp.setIdentity();
		temp.QuaternionToMatrix(&transform.rotation);
		m_world *= temp;

		temp.setIdentity();
		temp.setTranslation(transform.position);
		m_world *= temp;
	};

	Matrix4x4 GetWorldMatrix()
	{
		return m_world;
	};

	template <class T>
	T* GetComponent()
	{
		const char* num = typeid(T).name();
		for (int i = 0; i < componentsNumber; i++)
		{
			if (num == componentsId[i])
			{
				return std::static_pointer_cast<T>(mComponentArrays[i]).get();
			}
		}
		return nullptr;
	};

	void UpdateAll()
	{
		for (int i2 = 0; i2 < componentsNumber; i2++)
		{
			if (mComponentArrays[i2]->enabled)
			{
				mComponentArrays[i2]->gameobject = this;
				mComponentArrays[i2]->Update();
			}
		}
	}

	template<typename T>
	void AddComponent()
	{
		mComponentArrays[componentsNumber] = std::make_shared<T>();
		mComponentArrays[componentsNumber]->gameobject = this;
		componentsId[componentsNumber] = typeid(T).name();
		componentsNumber += 1;
	}

	Vector3 forward()
	{
		return m_world.getZDirection();
	}

	~GameObject()
	{

	};
private:
	std::shared_ptr<Component> mComponentArrays[MAX_ENTITIES] = {};
	const char* componentsId[MAX_ENTITIES] = {};
};



inline GameObject* GameObjectManager::AddGameObject(Vector3 position, Quaternion quaternion, Vector3 scale)
{
	list_of_objects.push_back(GameObject());
	std::list<GameObject>::iterator it = list_of_objects.begin();
	std::advance(it, number_of_objects);
	(*it).transform.parent = nullptr;
	(*it).transform.position = position;
	(*it).transform.rotation = quaternion;
	(*it).transform.scale = scale;

	number_of_objects += 1;
	return &(*it);
}

inline void GameObjectManager::SetCamera(GameObject* camera)
{
	this->camera = camera;
}