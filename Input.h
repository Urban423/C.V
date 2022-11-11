#pragma once
#include "Point.h"
#include "Cursor.h"
#include <iostream>
#include "InputHelper.h"

class Input
{
public:
	static bool GetKeyDown(int number)
	{
		if (InputHelper::m_keyboard_state[number] == 1)
		{
			return true;
		}
		return false;
	}
	static bool GetKeyUp(int number)
	{
		if (InputHelper::m_keyboard_state[number] == 2)
		{
			return true;
		}
		return false;
	}
	static bool GetKey(int number)
	{
		if (InputHelper::m_keyboard_state[number] == 3)
		{
			return true;
		}
		return false;
	}
	static float GetAxisX()
	{
		POINT current_mouse_pos = {};
		::GetCursorPos(&current_mouse_pos);
		return current_mouse_pos.x - Cursor::getWindowCenter().x;
	}
	static float GetAxisY()
	{
		POINT current_mouse_pos = {};
		::GetCursorPos(&current_mouse_pos);
		return current_mouse_pos.y - Cursor::getWindowCenter().y;
	}
};