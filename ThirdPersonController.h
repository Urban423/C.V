#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;

class ThirdPersonController : public Component
{
public:
	POINT startCursorPos;
	POINT cursorPos;
	POINT deltaCursorsPos;
	Vector3 angle;
	POINT screen;
public:
	void Update()
	{
		if (InputSystem::get()->m_mouse_wheel > 0)
		{
			InputSystem::get()->m_mouse_wheel = 0;
		}
		gameobject->transform.localPosition = Vector3(0, 0, 1) * InputSystem::get()->m_mouse_wheel * -InputSystem::get()->m_mouse_wheel;
		if (Input::GetKeyDown(4) || Input::GetKeyDown(2))
		{
			screen = Cursor::getScreenSize();
			startCursorPos = Cursor::getCursorPos();
		}
		if (Input::GetKey(4) || Input::GetKey(2))
		{
			cursorPos = Cursor::getCursorPos();

			if (cursorPos.x == 0)
			{
				startCursorPos.x = 0;
				Cursor::setCursorPosition(screen.x, cursorPos.y);
			}
			else if (cursorPos.x == screen.x - 1)
			{
				startCursorPos.x = screen.x - 1;
				Cursor::setCursorPosition(0, cursorPos.y);
			}

			if (cursorPos.y == 0)
			{
				startCursorPos.y = 0;
				Cursor::setCursorPosition(cursorPos.x, screen.y);
			}
			else if (cursorPos.y == screen.y - 1)
			{
				startCursorPos.y = screen.y - 1;
				Cursor::setCursorPosition(cursorPos.x, 0);
			}

			deltaCursorsPos.x = cursorPos.x - startCursorPos.x;
			deltaCursorsPos.y = cursorPos.y - startCursorPos.y;

			if (Input::GetKey(160))
			{
				if (gameobject->transform.parent->transform.parent) {
					gameobject->transform.parent->transform.rotation = gameobject->transform.parent->transform.parent->transform.rotation * gameobject->transform.parent->transform.localRotation;
					gameobject->transform.parent->transform.parent = nullptr;
					angle = gameobject->transform.parent->transform.rotation.QuarToVector3();
				}
				Matrix4x4 world_cam, temp;
				world_cam.setIdentity();
				temp.setIdentity();
				temp.QuaternionToMatrix(&(gameobject->transform.parent->transform.rotation));
				world_cam *= temp;
				world_cam.setTranslation(gameobject->transform.parent->transform.position);
				gameobject->transform.parent->m_world = world_cam;

				gameobject->transform.parent->transform.position = gameobject->transform.parent->transform.position + gameobject->transform.parent->m_world.getXDirection() * -deltaCursorsPos.x * 1.2f +
					gameobject->transform.parent->m_world.getYDirection() * deltaCursorsPos.y * 1.2f;
			}
			else 
			{
				int i = 1;
				if (angle.z > 155 || angle.z < -155)
				{
					i = -1;
				}
				angle.y += (float)deltaCursorsPos.x * 0.3f * i;
				angle.x += (float)deltaCursorsPos.y * 0.3f;

				if (gameobject->transform.parent->transform.parent)
				{
					gameobject->transform.parent->transform.localRotation = angle;
				}
				else {
					gameobject->transform.parent->transform.rotation = angle;
				}
			}
			startCursorPos = cursorPos;
		}
	}
};