#pragma once
#include <Windows.h>
#include "AppWindow.h"
#include "Vector2.h"
#include "Rect.h"

class Cursor
{
public:
	static bool lockCursor;
public:
	static void showCursor(const bool& visible)
	{
		::ShowCursor(visible);
	}
	static void setCursorPosition(const int& x, const int& y)
	{
		::SetCursorPos(x, y);
	};
	static void setCursorToCenter()
	{
		::SetCursorPos(win_size.left + (int)(win_size.width / 2.0f), win_size.top + (int)(win_size.height / 2.0f));
	}
	static Vector2 getWindowCenter()
	{
		Vector2 center;
		center.x = win_size.left + (int)(win_size.width / 2.0f);
		center.y = win_size.top + (int)(win_size.height / 2.0f);
		return center;
	}

	static POINT getCursorPos()
	{
		POINT current_mouse_pos = {};
		::GetCursorPos(&current_mouse_pos);
		return current_mouse_pos;
	}
	static POINT getScreenSize()
	{
		POINT win;
		win.x = screen.width;
		win.y = screen.height;
		return win;
	}
	static POINT getWindowSize()
	{
		POINT win;
		win.x = win_size.width;
		win.y = win_size.height;
		return win;
	}
private:
	static Rect win_size;
	static Rect screen;

	friend class AppWindow;
};