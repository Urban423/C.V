#pragma once
#include <iostream>
#include <Windows.h>
#include "AppWindow.h"

class InputHelper 
{
public:
	static void Update()
	{
		if (GetKeyboardState(m_new_keyboard_state))
		{
			for (int i = 0; i < 256; i++)
			{
				InputHelper::m_keyboard_state[i] = 0;
				if (m_old_keyboard_state[i] & 0x80)
				{

				}
				else
				{
					if (m_new_keyboard_state[i] != m_old_keyboard_state[i])
					{
						InputHelper::m_keyboard_state[i] = 1;
						continue;
					}
				}

				if (m_new_keyboard_state[i] & 0x80)
				{
					InputHelper::m_keyboard_state[i] = 3;
				}
				else
				{
					if (m_new_keyboard_state[i] != m_old_keyboard_state[i])
					{
						InputHelper::m_keyboard_state[i] = 2;
					}
				}
			}
		}
		::memcpy(m_old_keyboard_state, m_new_keyboard_state, sizeof(BYTE) * 256);
	}
	static BYTE m_old_keyboard_state[256];
	static BYTE m_new_keyboard_state[256];

	static int m_keyboard_state[256];
};