#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;
#include <iostream>

class Ray: public Component
{
public:
	bool f = false;
	float distance = 3; 
	GameObject* rocket;
private:
	void Update()
	{
		if (Input::GetKeyDown(1))
		{
			POINT win = Cursor::getWindowSize();
			float CursorsPos_x = Input::GetAxisX() + win.x / 4 / 2;
			win.x = win.x / 4 * 3;
			float CursorsPos_y = Input::GetAxisY();
			RaycastHit hit; 
			
			Vector2 relative = Vector2(
				CursorsPos_x  / win.x,
				CursorsPos_y / win.y
			);

			float verticalAngle = 0.5f;
			float worldHeight = 2 * tan(verticalAngle);

			relative = relative * worldHeight;
			relative.x *= ((float)win.x / (float)win.y);
			Vector3 direction = gameobject->m_world.getZDirection() + gameobject->m_world.getXDirection() * relative.x + gameobject->m_world.getYDirection() * -(relative.y);
			if (Physics::Raycast(gameobject->transform.position, direction, 333000000, &hit))
			{
				gameobject->transform.parent->transform.parent = hit.hitObject;
				gameobject->transform.parent->transform.localRotation = Quaternion::Conjugate(hit.hitObject->transform.rotation) * gameobject->transform.parent->transform.rotation;
				gameobject->GetComponent<ThirdPersonController>()->angle = gameobject->transform.parent->transform.localRotation.QuarToVector3();
				if (hit.hitObject->GetComponent<Take>())
				{
					hit.hitObject->GetComponent<Take>()->Do();
				}
				if (hit.hitObject->GetComponent<BoxCollider>())
				{
					return;
				}
				if (f)
				{
					f = false;
					Vector3 pr = hit.point + hit.normal * 2 - hit.hitObject->transform.position;
					Vector3 rot = hit.hitObject->transform.rotation.QuarToVector3();
					Quaternion q = -rot;
					pr = rocket->transform.parent->transform.rotation.QuanRotation2(pr, q);
					rocket->GetComponent<Rocket>()->normal = pr;
					return;
				}
				rocket = GameObjectManager::get()->AddGameObject(Vector3(), Quaternion(Vector3(0, 0, 0)), Vector3(0.3f, 0.3f, 0.3f));


				rocket->transform.parent = hit.hitObject;
				Vector3 pr = hit.point + hit.normal * 2 - hit.hitObject->transform.position;
				Vector3 rot = hit.hitObject->transform.rotation.QuarToVector3();
				Quaternion q = -rot;
				pr = rocket->transform.parent->transform.rotation.QuanRotation2(pr,q);
				rocket->transform.localPosition = pr;

				rocket->AddComponent<Rocket>();
				f = true;

				rocket->AddComponent<MeshRenderer>();
				rocket->GetComponent<MeshRenderer>()->number = 2;
				rocket->GetComponent<MeshRenderer>()->AddMaterial(0);
			}
		}
	}
};