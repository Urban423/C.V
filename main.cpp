#include "AppWindow.h"

int main() {
	try
	{
		GameObjectManager::create();
		InputSystem::create();
		GraphicsEngine::create();
	}
	catch (...) { return -1; }

	{
		try {
			AppWindow app;
			while (app.isRunning());
		}
		catch (...) {

			InputSystem::release();
			GraphicsEngine::release();
			return -1;
		}
	}

	GameObjectManager::release();
	InputSystem::release();
	GraphicsEngine::release();
	return 0;
}