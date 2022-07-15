#include "AppWindow.h"
#include <Windows.h>
#include "Quaternion.h"
#include "InputSystem.h"
#include "BoxCollider.h"
#include "MeshRenderer.h"
#include "Rotate.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Component.h"
#include <iostream>
using namespace UrbanEngine;
using namespace ZipolorEngine;

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_light_direction;
	Vector4 m_camera_position;
	Vector4 m_light_position = Vector4(0, 1, 0, 0);
	float m_light_radius = 555;
};

void AppWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0, 0, 0, 1);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateCamera();

	for (int i = 0; i < numbe_of_objects; i++)
	{
		if (!items[i].enabled) {
			continue;
		}

		gameobject = items[i];
		gameobject.UpdateAll();
		items[i] = gameobject;

		Rigidbody* rig = items[i].GetComponent<Rigidbody>();
		if (rig && Time.delta_time != 0)
		{
			items[i].transform.position = items[i].transform.position + physics.Gravity * Time.delta_time * 0.3f;
			for (int i1 = 1; i1 < number_of_colliders; i1++)
			{
				if (i != colliders[i1])
				{
					items[i].transform.position = items[i].transform.position - BoxCollider::onCollisonEnter(items[i], items[colliders[i1]]);
				}
			}
			if (Vector3::distance(rig->lastpos, items[i].transform.position) == 0)
			{
				rig->idle = true;
			}
			rig->lastpos = items[i].transform.position;
		}
	}

	MeshRenderer* meshRenderer;
	for (int i = 0; i < numbe_of_objects; i++)
	{
		if (!items[i].enabled) {
			continue;
		}

		meshRenderer = items[i].GetComponent<MeshRenderer>();
		if (meshRenderer)
		{
			material_list.clear();
			for (int i1 = 0; i1 < meshRenderer->sizeMaterialNumbers; i1++)
			{
				material_list.push_back(m_materials[meshRenderer->MaterialNumbers[i1]]);
			}
			updateModel(&items[i], material_list);
			drawMesh(m_meshes[meshRenderer->number], material_list);
		}
	}

	m_swap_chain->present(true);

	UrbanEngine::Time.TimeUpdate();
	if (pause)
	{
		Time.delta_time = 0;
	}
}

void AppWindow::updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials)
{
	constant cc;

	obj->processWorldMatrix();

	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setIdentity();
	m_light_roy_t += 0.707f * Time.delta_time;
	m_light_rot_matrix.SetRotationY(m_light_roy_t);
	cc.m_world = obj->GetWorldMatrix();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	cc.m_light_radius = 6;
	cc.m_light_position = Vector4(0, 1, 0, 0);

	for (int i = 0; i < materials.size(); i++)
	{
		materials[i]->setData(&cc, sizeof(constant));
	}
}

void AppWindow::updateCamera()
{

	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.SetRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.SetRotationY(m_rot_y);
	world_cam *= temp;

	Vector3 new_pos;
	Vector3 finaly = items[0].transform.position;

	if (Time.delta_time > 0)
	{
		Vector3 ter = world_cam.getZDirection();
		ter.y = 0;
		new_pos = new_pos + ter * forward;
		new_pos = new_pos + world_cam.getXDirection() * right;

		items[0].transform.position = items[0].transform.position + new_pos.normalized() * 0.1f + Vector3(0, 1, 0) * up;

		finaly = items[0].transform.position;
	}

	world_cam.setTranslation(finaly);
	m_world_cam = world_cam;

	world_cam.inverse();

	m_view_cam = world_cam;

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	m_proj_cam.setPersepciveFovLH(1, ((float)width / (float)height), 0.03f, 1000);
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
	items.push_back(GameObject());
	items[numbe_of_objects].AddComponent<BoxCollider>();
	items[numbe_of_objects].GetComponent<BoxCollider>()->Center = Vector3(0, -1, 0);
	items[numbe_of_objects].GetComponent<BoxCollider>()->size = Vector3(0.3f, 1, 0.3f);
	items[numbe_of_objects].AddComponent<Rigidbody>();
	items[numbe_of_objects].transform.position = Vector3(0, 2, 0);
	colliders[number_of_colliders] = numbe_of_objects;
	number_of_colliders++;
	numbe_of_objects++;

	items.push_back(GameObject());
	items[numbe_of_objects].transform.position = Vector3(0, -1.5f, 0);
	items[numbe_of_objects].transform.scale = Vector3(100, 1, 100);
	items[numbe_of_objects].AddComponent<BoxCollider>();
	items[numbe_of_objects].GetComponent<BoxCollider>()->size = Vector3(100, 1, 100);
	colliders[number_of_colliders] = numbe_of_objects;
	number_of_colliders++;
	items[numbe_of_objects].AddComponent<MeshRenderer>();
	items[numbe_of_objects].GetComponent<MeshRenderer>()->number = 1;
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(1);
	numbe_of_objects += 1;

	items.push_back(GameObject());
	items[numbe_of_objects].transform.position = Vector3(2, 0, 0);
	items[numbe_of_objects].AddComponent<BoxCollider>();
	colliders[number_of_colliders] = numbe_of_objects;
	number_of_colliders++;
	items[numbe_of_objects].AddComponent<Rotate>();
	items[numbe_of_objects].GetComponent<Rotate>()->rotate = Vector3(0, 222, 222);
	items[numbe_of_objects].AddComponent<MeshRenderer>();
	items[numbe_of_objects].GetComponent<MeshRenderer>()->number = 1;
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(1);
	numbe_of_objects += 1;

	
	items.push_back(GameObject());
	items[numbe_of_objects].enabled = false;
	items[numbe_of_objects].AddComponent<MeshRenderer>();
	items[numbe_of_objects].GetComponent<MeshRenderer>()->number = 0;
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(0);
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(1);
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(2);
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(3);
	items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(4);
	numbe_of_objects += 1;
	

	Window::onCreate();
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	WhiteTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\White.png");
	OboiTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Oboi1.jpg");
	FloorTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Floor1.png");
	ValvesTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Valves.png");
	DoNotEnter = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clock.png");
	Sky = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.png");
	WallBrickTex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\WallsCanalization.png");

	m_meshes[0] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Canalization.obj");
	m_meshes[1] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\cube.obj");
	m_meshes[2] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\sphere.obj");
	m_meshes[3] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\clock.obj");
	m_meshes[4] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\plane.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	RECT win_size = this->getClientWindowRect();
	int width = (win_size.right - win_size.left);
	int height = (win_size.bottom - win_size.top);
	InputSystem::get()->setCursorPosition(Point(win_size.left + (int)(width / 2.0f), win_size.top + (int)(height / 2.0f)));


	m_materials[0] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\SimpleShader.hlsl");
	m_materials[0]->addTexture(WhiteTex);
	m_materials[0]->setCullMode(CULL_MODE_FRONT);

	m_materials[1] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[1]->addTexture(OboiTex);
	m_materials[1]->setCullMode(CULL_MODE_FRONT);

	m_materials[2] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[2]->addTexture(WallBrickTex);
	m_materials[2]->setCullMode(CULL_MODE_FRONT);

	m_materials[3] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[3]->addTexture(ValvesTex);
	m_materials[3]->setCullMode(CULL_MODE_FRONT);

	m_materials[4] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[4]->addTexture(FloorTex);
	m_materials[4]->setCullMode(CULL_MODE_FRONT);

	m_materials[5] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[5]->addTexture(DoNotEnter);
	m_materials[5]->setCullMode(CULL_MODE_FRONT);

	material_list.reserve(32);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->FullScreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right - rc.left, rc.bottom - rc.top);
	this->render();
}

void AppWindow::consoleWork()
{
	std::string comand;
	if (comand == "/gamemode")
	{
		show_console = false;
	}
	else if (comand == "/exit")
	{
		show_console = false;
	}
	else if (comand == "/stop")
	{

	}
	else
	{
		consoleWork();
	}
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		forward = 1.0f;
	}
	else if (key == 'S')
	{
		forward = -1.0f;
	}
	if (key == 'A')
	{
		right = -1.0f;
	}
	else if (key == 'D')
	{
		right = 1.0f;
	}
	if (key == 32)
	{
		up = 1.0f;
	}
	UrbanEngine::Input.keyDown = key;
}

void AppWindow::onKeyUp(int key)
{
	forward = 0.0f;
	right = 0.0f; 
	up = 0;
	if (key == 27)
	{
		if (pause)
		{
			RECT win_size = this->getClientWindowRect();
			int width = (win_size.right - win_size.left);
			int height = (win_size.bottom - win_size.top);
			InputSystem::get()->setCursorPosition(Point(win_size.left + (int)(width / 2.0f), win_size.top + (int)(height / 2.0f)));
		}
		pause = !pause;
		InputSystem::get()->showCursor(pause);
	}
	if (key == 115)
	{
		fullscreen = !fullscreen;
		RECT size_screen = this->getSizeScreen();
		m_swap_chain->FullScreen(fullscreen, size_screen.right, size_screen.bottom);
		if (!pause)
		{
			RECT win_size = this->getClientWindowRect();
			int width = (win_size.right - win_size.left);
			int height = (win_size.bottom - win_size.top);
			InputSystem::get()->setCursorPosition(Point(win_size.left + (int)(width / 2.0f), win_size.top + (int)(height / 2.0f)));
		}
		this->render();
	}
	if (key == 192)
	{
		show_console = true;
		
	}
	if (key == '2')
	{
		show = !show;
	}
	if (key == 'E')
	{
		items.push_back(GameObject());
		items[numbe_of_objects].transform.rotation = Quaternion(Vector3(0, 12, 0));
		items[numbe_of_objects].transform.scale = Vector3(1, 1, 1);
		items[numbe_of_objects].transform.position = m_world_cam.getTranslation() + m_world_cam.getZDirection() * 2;
		items[numbe_of_objects].AddComponent<MeshRenderer>();
		items[numbe_of_objects].GetComponent<MeshRenderer>()->number = 1;
		items[numbe_of_objects].GetComponent<MeshRenderer>()->AddMaterial(5);
		items[numbe_of_objects].AddComponent<BoxCollider>();
		items[numbe_of_objects].GetComponent<BoxCollider>()->size = Vector3(1, 1, 1);
		colliders[number_of_colliders] = numbe_of_objects;
		items[numbe_of_objects].AddComponent<Rigidbody>();
		items[numbe_of_objects].GetComponent<Rigidbody>()->mass = 2;
		items[numbe_of_objects].AddComponent<Rotate>();
		items[numbe_of_objects].GetComponent<Rotate>()->rotate = Vector3(222, 222, 222);
		number_of_colliders++;
		numbe_of_objects += 1;
	}

	if (key == 81) //Q
	{
		Quaternion q(Vector3(-55, -90, 21));
		std::cout << q.x << " " << q.y << " " << q.z << " " << q.w << "\n";

		Vector3 t = Quaternion::QuarToVector3(q);
		std::cout << t.x << " " << t.y << " " << t.z << "\n";
	}

	UrbanEngine::Input.keyUp = key;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if (!pause)
	{
		RECT win_size = this->getClientWindowRect();

		int width = (win_size.right - win_size.left);
		int height = (win_size.bottom - win_size.top);

		m_rot_x += (mouse_pos.y - (win_size.top + (height / 2.0f))) * Time.delta_time * 0.25f;

		m_rot_x = Math::clamp(m_rot_x, 1.5f, -1.5f);

		m_rot_y += (mouse_pos.x - (win_size.left + (width / 2.0f))) * Time.delta_time * 0.25f;
		InputSystem::get()->setCursorPosition(Point(win_size.left + (int)(width / 2.0f), win_size.top + (int)(height / 2.0f)));
	}
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{

}
