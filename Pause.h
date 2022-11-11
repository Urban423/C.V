#pragma once
using namespace UrbanEngine;
using namespace ZipolorEngine;


class Pause: public Component
{
public:
	bool pause = false;
public:
	void Update()
	{
		if (Input::GetKeyDown(27) || Input::GetKeyDown(32))
		{
			pause = !pause;
			//Cursor::showCursor(pause);
			//Cursor::lockCursor = !pause;
			if (pause)
			{
				Time::timescale = 0;
			}
			else 
			{
				//Cursor::setCursorToCenter();
				Time::timescale = 1;
			}
		}
	}
};