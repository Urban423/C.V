#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Scene
{
public:
	static void SetScene()
	{
		GameObject* gameobject;

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(), Quaternion(), Vector3(1, 1, 1));
		gameobject->AddComponent<Pause>();

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(), Quaternion(), Vector3(1, 1, 1));

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(0, 0, 0), Quaternion(), Vector3(1, 1, 1));

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(), Quaternion(), Vector3(1, 1, 1));
		gameobject->transform.localPosition = Vector3(0, 0, -10);
		gameobject->transform.parent = GameObjectManager::get()->GetbyIndex(2);
		gameobject->AddComponent<ThirdPersonController>(); gameobject->AddComponent<Camera>();
		gameobject->AddComponent<Ray>();


		gameobject = GameObjectManager::get()->AddGameObject(Vector3(0, 0, 0), Quaternion(Vector3(66, 0, 66)), Vector3(0.3f, 0.3f, 0.3f)); 
		gameobject->AddComponent<BoxCollider>();
		gameobject->GetComponent<BoxCollider>()->size = Vector3(3, 12, 3);
		gameobject->AddComponent<MeshRenderer>();
		gameobject->GetComponent<MeshRenderer>()->number = 7;
		gameobject->GetComponent<MeshRenderer>()->AddMaterial(6);

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(0, 0, 4000), Quaternion(), Vector3(100, 100, 100));
		gameobject->AddComponent<MeshRenderer>();
		gameobject->GetComponent<MeshRenderer>()->number = 2;
		gameobject->GetComponent<MeshRenderer>()->AddMaterial(9);
		gameobject->AddComponent<SphereCollider>();
		gameobject->AddComponent<Light>();
		gameobject->GetComponent<Light>()->radius = 10000;
		gameobject->AddComponent<Rigidbody>();
		gameobject->GetComponent<Rigidbody>()->mass = 10000000000;
		gameobject->GetComponent<Rigidbody>()->isKinematic = true;
		gameobject->GetComponent<SphereCollider>()->radius = 100;
		gameobject->AddComponent<Rotate>();
		gameobject->GetComponent<Rotate>()->rotate.y = -5;

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(0,0,0), Quaternion(), Vector3(40, 40, 40));
		gameobject->AddComponent<MeshRenderer>();
		gameobject->GetComponent<MeshRenderer>()->number = 2;
		gameobject->GetComponent<MeshRenderer>()->AddMaterial(4);
		gameobject->AddComponent<SphereCollider>();
		gameobject->AddComponent<Rigidbody>();
		gameobject->GetComponent<Rigidbody>()->isKinematic = true;
		gameobject->GetComponent<Rigidbody>()->velocity = Vector3(2.5f,0,0);
		gameobject->AddComponent<Planet>();
		gameobject->GetComponent<Planet>()->sun= GameObjectManager::get()->GetbyIndex(5);
		gameobject->GetComponent<SphereCollider>()->radius = 50;
		gameobject->AddComponent<Rotate>();
		gameobject->GetComponent<Rotate>()->rotate.y = 1;

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(500, 0, 0), Quaternion(), Vector3(10, 10, 10));
		gameobject->transform.parent = GameObjectManager::get()->GetbyIndex(6);
		gameobject->transform.localPosition = Vector3(300, 0, 0);
		gameobject->AddComponent<MeshRenderer>();
		gameobject->GetComponent<MeshRenderer>()->number = 2;
		gameobject->GetComponent<MeshRenderer>()->AddMaterial(8);
		gameobject->AddComponent<SphereCollider>();
		gameobject->GetComponent<SphereCollider>()->radius = 10;

		gameobject = GameObjectManager::get()->AddGameObject(Vector3(), Quaternion(), Vector3(100000 - 1, 100000 - 1, 100000 - 1));
		gameobject->AddComponent<Sky>();
		gameobject->GetComponent<Sky>()->Cam = GameObjectManager::get()->GetbyIndex(3);
		gameobject->AddComponent<MeshRenderer>();
		gameobject->GetComponent<MeshRenderer>()->number = 2;
		gameobject->GetComponent<MeshRenderer>()->AddMaterial(7);
	};
};