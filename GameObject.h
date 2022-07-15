#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Component.h"

const int MAX_ENTITIES = 32;

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
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
		transform.rotation = Quaternion(Vector3(0, 0, 0));
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
				mComponentArrays[i2]->Update();
			}
		}
	}

	template<typename T>
	void AddComponent()
	{
		mComponentArrays[componentsNumber] = std::make_shared<T>();
		componentsId[componentsNumber] = typeid(T).name();
		componentsNumber += 1;
	}

	~GameObject()
	{

	};
private:
	std::shared_ptr<Component> mComponentArrays[MAX_ENTITIES] = {};
	const char* componentsId[MAX_ENTITIES] = {};
};
