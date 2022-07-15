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
#include "BoxCollider.h"
#include <vector>
#include "Physics.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "GameObject.h"

class AppWindow : public Window, public InputListener
{
public:
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;
	virtual void onSize() override;

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
public:
	void render();
	void updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials);
	void updateCamera();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);
	void consoleWork();
private:
	SwapChainPtr m_swap_chain;

	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_sky_cb;

	TexturePtr DoNotEnter;
	TexturePtr WallBrickTex;
	TexturePtr OboiTex;
	TexturePtr FloorTex;
	TexturePtr ValvesTex;
	TexturePtr WhiteTex;
	TexturePtr Sky;

	MaterialPtr m_materials[100] = {};
	int number_of_materials = 0;

	MeshPtr m_meshes[100] = {};
	int number_of_meshes = 0; 

	int colliders[100] = {};
	int number_of_colliders = 0;

	std::vector<int> physicupdate = {};
	Physics physics;

	std::vector<GameObject> items = {};
	int numbe_of_objects = 0;
private:
	bool show = false;
	bool show_console = false;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_light_roy_t;

	bool pause = false;
	bool console = false;
	bool fullscreen = false;

	float forward = 0.0f;
	float right = 0.0f;
	float up = 0.0f;
	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	std::vector<MaterialPtr> material_list;
};