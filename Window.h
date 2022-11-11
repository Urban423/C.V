#pragma once
#include <Windows.h>
#include "InputSystem.h"

class Window
{
public:
	Window();
	bool isRunning();

	RECT getClientWindowRect();
	RECT getSizeScreen();
	HWND getWindow();

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();
	virtual void onMove();

	~Window();
	int m_mouse_wheel;
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};

