#include "AppWindow.h"
#include <Windows.h>
#include "InputSystem.h"
#include "Cursor.h"
#include "Rect.h"
#include "Input.h"
Rect Cursor::win_size = Rect();
Rect Cursor::screen = Rect();
bool Cursor::lockCursor = false;

__declspec(align(16))
struct constantMain
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_camera_position;
};

void AppWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 1, 1, 1, 1);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	material_list.clear();
	material_list.push_back(post_process_mat);
	drawMesh(quad, material_list);
	material_list.clear();
	material_list.push_back(console_mat);
	drawMesh(quad1, material_list);
	
	m_swap_chain->present(true);
}

void AppWindow::updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials)
{
	constantMain cc;

	obj->processWorldMatrix();

	cc.m_world = obj->GetWorldMatrix();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();

	for (int i = 0; i < materials.size(); i++)
	{
		materials[i]->setData(&cc, sizeof(constantMain));
	}
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	Quaternion t = Quaternion(Vector3(0, 0, 0));
	temp.QuaternionToMatrix(&t);
	world_cam *= temp;

	world_cam.setTranslation(Vector3(0, 0, -2));
	m_world_cam = world_cam;

	world_cam.inverse();
	m_view_cam = world_cam;

	RECT rc = this->getClientWindowRect();
	float width = rc.right - rc.left;
	float height = rc.bottom - rc.top;
	m_proj_cam.setOrthoLH(width, height, 0.03f, 1000);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());
	for (size_t m = 0; m < mesh->getNumMaterialSlots(); m++)
	{
		if (m >= materials.size()) break;

		MaterialSlot mat_slot = mesh->getMaterialSlot(m);
		GraphicsEngine::get()->setMaterial(materials[m]);
		GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTrianglesList(mat_slot.num_indices, 0, mat_slot.start_index);
	}
}

void AppWindow::onCreate()
{
	Window::onCreate();
	Cursor::win_size = this->getClientWindowRect();
	Cursor::showCursor(true);

	RECT rc = this->getClientWindowRect();
	Cursor::screen = this->getSizeScreen();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	int width = (rc.right - rc.left);
	int height = (rc.bottom - rc.top);
	InputSystem::get()->setCursorPosition(Point(rc.left + (int)(width / 2.0f), rc.top + (int)(height / 2.0f)));

	uv.window_size = rc;
	uv.window_size.width = uv.window_size.width / 4 * 3;
	uv.onCreate();
	uv.onUpdate();

	console.window_size = rc;
	console.window_size.width = console.window_size.width / 4 * 1;
	console.window_size.height = console.window_size.height;
	console.onCreate();
	console.onUpdate();

	game.window_size = rc;
	game.window_size.width = game.window_size.width / 4 * 3;
	game.onCreate();
	game.onUpdate();

	post_process_mat = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\PostProcessVS.hlsl");
	post_process_mat->addTexture(game.render_target);
	post_process_mat->addTexture(uv.render_target);
	post_process_mat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	console_mat = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\PostProcessVS.hlsl");
	console_mat->addTexture(console.render_target);
	console_mat->setCullMode(CULL_MODE::CULL_MODE_BACK);

	material_list.reserve(32);

	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3(-1, -1, 0), Vector2(0, 1), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(-1,  1, 0), Vector2(0, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3( 0.5,  1, 0), Vector2(1, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3( 0.5, -1, 0), Vector2(1, 1), Vector3(),  Vector3(),  Vector3())
	};

	unsigned int quad_index_list[] = {
		0, 1, 2,
		2, 3, 0
	};

	MaterialSlot quad_mat_slots[] = {
		{0, 6, 0}
	};

	quad = GraphicsEngine::get()->getMeshManager()->createMesh(
		quad_vertex_list, 4,
		quad_index_list, 6,
		quad_mat_slots, 1);

	VertexMesh quad_vertex_list1[] =
	{
		VertexMesh(Vector3(0.5, -1, 0), Vector2(0, 1), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(0.5,  1, 0), Vector2(0, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(1,  1, 0), Vector2(1, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(1, -1, 0), Vector2(1, 1), Vector3(),  Vector3(),  Vector3())
	};

	quad1 = GraphicsEngine::get()->getMeshManager()->createMesh(
		quad_vertex_list1, 4,
		quad_index_list, 6,
		quad_mat_slots, 1);

	updateCamera();

	material_list.clear();
	material_list.push_back(post_process_mat);
	updateModel(&item, material_list);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputHelper::Update();
	for (int i = 0; i < 256; i++)
	{
		if (Input::GetKeyDown(i))
		{
			onKeyUp(i);
		}
	}

	uv.onUpdate();
	console.onUpdate();
	game.onUpdate();

	if (Cursor::lockCursor)
	{
		Cursor::setCursorToCenter();
	}
	this->render();

	InputSystem::get()->update();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->FullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{

}

void AppWindow::onKillFocus()
{

}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);

	game.window_size = rc;
	game.window_size.width = game.window_size.width / 4 * 3;
	game.onSize();

	uv.window_size = rc;
	uv.window_size.width = uv.window_size.width / 4 * 3;
	uv.onSize();

	console.window_size = rc;
	console.window_size.width = console.window_size.width / 4 * 1;
	console.window_size.height = console.window_size.height;
	console.onSize();

	console_mat->setTexture(console.render_target, 0);
	post_process_mat->setTexture(game.render_target, 0);
	post_process_mat->setTexture(uv.render_target, 1);

	Cursor::win_size = rc;
	this->render();
}

void AppWindow::onMove()
{
	RECT rc = this->getClientWindowRect();
	Cursor::win_size = rc;
	this->render();
}

void AppWindow::onKeyUp(int key)
{
	if (key == 115)
	{
		fullscreen = !fullscreen;
		RECT size_screen = this->getSizeScreen();
		m_swap_chain->FullScreen(fullscreen, size_screen.right, size_screen.bottom);
		RECT win_size = this->getClientWindowRect();
		Cursor::win_size = win_size;
		Cursor::setCursorToCenter();
	}
	if (key == 112)
	{
		onDestroy();
	}
	if (key == 192)
	{
		show_console = true;
	}
	game.onKeyUp(key);
}