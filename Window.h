#pragma once
#include <Windows.h>

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

	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};

