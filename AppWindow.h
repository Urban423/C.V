#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include <vector>
#include "Console.h"
#include "Game.h"

class AppWindow : public Window, public InputListener
{
public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;
	virtual void onMove() override;

	virtual void onKeyUp(int key) override;
public:
	void render();
	void updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials);
	void updateCamera();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);
private:
	SwapChainPtr m_swap_chain;
	ConstantBufferPtr m_cb;

	std::vector<MaterialPtr> material_list;

	GameObject item;
	MaterialPtr post_process_mat;
	MaterialPtr console_mat;

	bool show_console = false;
	bool fullscreen = false;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	MeshPtr quad;
	MeshPtr quad1;

	Console console;
	UV uv;
	Game game;
private:
	friend class GameObject;
};