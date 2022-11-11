#pragma once
#include <exception>
#include "Point.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();
public:
	void update();
	void setCursorPosition(const Point& pos);

	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
public:
	static InputSystem* get();
	static void create();
	static void release();

	int m_mouse_wheel;
private:
	bool m_first_time = true;
	static InputSystem* m_system;
};

