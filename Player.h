#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class Player : public Component
{
public:
	float speed = 5;
	float runSpeed = 9;
	float jump = 6;
	GameObject* camera;
private:
	float x = 0;
	float y = 0;
	float moveSpeed;
	Vector3 move;
	float horizontal = 0;
	float vertical = 0;
public:
	void Update()
	{
		if (Input::GetKey(87))
		{
			horizontal = 1;
		}
		else if(Input::GetKey(83))
		{
			horizontal = -1;
		}
		else
		{
			horizontal = 0;
		}
		if (Input::GetKey(68))
		{
			vertical = 1;
		}
		else if (Input::GetKey(65))
		{
			vertical = -1;
		}
		else
		{
			vertical = 0;
		}
		if (Input::GetKey(160))
		{
			moveSpeed = runSpeed;
		}
		else
		{
			moveSpeed = speed;
		}
		if (Input::GetKeyDown(32))
		{
			gameobject->GetComponent<Rigidbody>()->velocity = Vector3(0, 9.8f, 0) * jump * 0.15f;
		}

		move = camera->m_world.getZDirection() * horizontal + camera->m_world.getXDirection() * vertical;
		move.y = 0;
		move = move.normalized() * moveSpeed;
		gameobject->transform.position = gameobject->transform.position + move * Time::delta_time;

		x += Input::GetAxisX() * Time::delta_time * 8;
		y += Input::GetAxisY() * Time::delta_time * 8;
		y = Math::clamp(y, 90, -90);

		gameobject->transform.rotation = Vector3(0, x, 0);
		camera->transform.localRotation = Vector3(y, 0, 0);
	}
};