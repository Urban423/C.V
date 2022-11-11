#include "Game.h"
#include "MeshRenderer.h"
#include "Rigidbody.h"
#include "Light.h"
#include "Camera.h"
#include "TimeSystem.h"
#include <iostream>
float Time::delta_time = 0.3f;
float Time::timescale = 1;
long Time::old_delta = 0.3f;
long Time::new_delta = 1;

#include "InputHelper.h"
BYTE InputHelper::m_old_keyboard_state[256];
BYTE InputHelper::m_new_keyboard_state[256];
int InputHelper::m_keyboard_state[256];

#include "Cursor.h"
#include "Rect.h"

#include "UrbanEngine.h"
#include "ZipolorEngine.h"

__declspec(align(16))
struct light
{
	Vector4 lightColor;
	Vector4 lightPosition;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_camera_position;
	light m_lights[32];
	float _Time;
};

void Game::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->render_target, 0, 0, 0, 1);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(
		window_size.width,
		window_size.height);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(this->render_target, this->depth_stencil);

	Time::TimeUpdate();
	time += Time::delta_time;

	std::list<GameObject>::iterator it = items->begin();
	for (int i = 0; i < number_of_objects; i++)
	{
		if (!(*it).enabled)
		{
			std::advance(it, 1);
			continue;
		}

		(*it).UpdateAll();
		number_of_objects = GameObjectManager::get()->NumberOfGameObjects();


		Rigidbody* rig = (*it).GetComponent<Rigidbody>();
		if (rig && Time::delta_time != 0 && !rig->isKinematic && (*it).GetComponent<BoxCollider>() && (*it).GetComponent<BoxCollider>()->enabled)
		{
			(*it).transform.position = (*it).transform.position + rig->velocity * Time::delta_time + Vector3(0, -9.81f, 0) * Time::delta_time * Time::delta_time * 0.5f;
			rig->velocity = rig->velocity + Vector3(0, -9.81f, 0) * Time::delta_time * 3;

			std::list<GameObject>::iterator its = items->begin();
			for (int i1 = 0; i1 < number_of_objects; i1++)
			{
				if (i != i1)
				{
					Vector3 collision;
					if ((*its).GetComponent<BoxCollider>())
					{
						collision = BoxCollider::onCollisonEnter((*it), (*its));
					}
					else if ((*its).GetComponent<SphereCollider>())
					{
						collision = SphereCollider::onCollisonEnter((*it), (*its));
					}
					if ((*its).GetComponent<Rigidbody>())
					{
						collision = collision * 0.6f;
					}
					if (collision != Vector3(0, 0, 0))
					{
						rig->velocity = Vector3(0, 0, 0);
					}
					(*it).transform.position = (*it).transform.position - collision;
				}
				std::advance(its, 1);
			}
		}
		std::advance(it, 1);
	}

	it = items->begin();
	for (int i = 0; i < number_of_objects; i++)
	{

		if ((*it).transform.parent != nullptr)
		{
			(*it).transform.rotation = (*it).transform.parent->transform.rotation * (*it).transform.localRotation;
			(*it).transform.position = (*it).transform.parent->transform.position + (*it).transform.parent->transform.rotation.QuanRotation2((*it).transform.localPosition, (*it).transform.parent->transform.rotation);
		}


		std::advance(it, 1);
	}

	updateCamera();

	it = items->begin();

	MeshRenderer* meshRenderer;
	for (int i = 0; i < number_of_objects; i++)
	{
		meshRenderer = (*it).GetComponent<MeshRenderer>();
		if (!(*it).enabled || !meshRenderer || !meshRenderer->enabled) {
			std::advance(it, 1);
			continue;
		}
		material_list.clear();
		for (int i1 = 0; i1 < meshRenderer->sizeMaterialNumbers; i1++)
		{
			material_list.push_back(m_materials[meshRenderer->MaterialNumbers[i1]]);
		}
		updateModel(&(*it), material_list);
		drawMesh(m_meshes[meshRenderer->number], material_list);
		std::advance(it, 1);
		m_materials[10]->setTexture(render_target, 0);
	}
}

void Game::updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials)
{
	constant cc;

	obj->processWorldMatrix();

	cc.m_world = obj->GetWorldMatrix();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc._Time = time;

	int light_number = 0;
	std::list<GameObject>::iterator it = items->begin();
	for (int i = 0; i < number_of_objects; i++)
	{
		if ((*it).GetComponent<Light>())
		{
			cc.m_lights[light_number].lightColor = Vector4((*it).GetComponent<Light>()->color.Color3(), (*it).GetComponent<Light>()->radius);
			cc.m_lights[light_number].lightPosition = Vector4((*it).transform.position, 1);
			light_number++;
		}
		std::advance(it, 1);
	}

	for (int i = 0; i < materials.size(); i++)
	{
		materials[i]->setData(&cc, sizeof(constant));
	}
}

void Game::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.QuaternionToMatrix(&(cam->transform.rotation));
	world_cam *= temp;

	world_cam.setTranslation(cam->transform.position);
	m_world_cam = world_cam;
	cam->m_world = world_cam;

	world_cam.inverse();
	m_view_cam = world_cam;

	m_proj_cam.setPersepciveFovLH(1, (float)window_size.width / (float)window_size.height, 0.03f, 500000);
}

void Game::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
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

void Game::onCreate()
{
	ZipolorEngine::Scene::SetScene();

	number_of_objects = GameObjectManager::get()->NumberOfGameObjects();
	items = GameObjectManager::get()->ListOfGameObjects();

	std::list<GameObject>::iterator it = items->begin();
	for (int i = 0; i < number_of_objects; i++)
	{
		if ((*it).GetComponent<Camera>())
		{
			GameObjectManager::get()->SetCamera(&(*it));
			cam = &(*it);
			break;
		}
		std::advance(it, 1);
	}

	Time::CreateTime();

	m_textures[0] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\White.png");
	m_textures[1] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Oboi1.jpg");
	m_textures[2] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Floor1.png");
	m_textures[3] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Valves.png");
	m_textures[4] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\clock.png");
	m_textures[5] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\planet.png");
	m_textures[6] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\sky.png");
	m_textures[7] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\WallsCanalization.png");
	m_textures[8] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\WallsCanalizationNM.png");
	m_textures[9] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Rocket.png");
	m_textures[10] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Black.png");
	m_textures[11] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\stars.jpg");
	m_textures[12] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\moon.png");
	m_textures[13] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Sun.png");
	m_textures[14] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Earth_normal_map.png");
	m_textures[15] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Earth_clouds.png");
	m_textures[16] = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\pointer.png");

	m_meshes[0] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Canalization.obj");
	m_meshes[1] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\cube.obj");
	m_meshes[2] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\sphere.obj");
	m_meshes[3] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\clock.obj");
	m_meshes[4] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\plane.obj");
	m_meshes[5] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Triangle.obj");
	m_meshes[6] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Rocket.obj");
	m_meshes[7] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Rocket.obj");
	m_meshes[8] = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets\\Models\\Line.obj");


	float scale = 42;

	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3(-scale, -scale, 0), Vector2(0, 1), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(-scale,  scale, 0), Vector2(0, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(scale,  scale, 0), Vector2(1, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(scale, -scale, 0), Vector2(1, 1), Vector3(),  Vector3(),  Vector3())
	};

	unsigned int quad_index_list[] = {
		0, 1, 2,
		2, 3, 0
	};

	MaterialSlot quad_mat_slots[] = {
		{0, 6, 0}
	};

	m_meshes[6] = GraphicsEngine::get()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);

	m_materials[0] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\SimpleShader.hlsl");
	m_materials[0]->addTexture(m_textures[0]);

	m_materials[1] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[1]->addTexture(m_textures[1]);
	m_materials[1]->addTexture(m_textures[8]);

	m_materials[2] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[2]->addTexture(m_textures[3]);

	m_materials[3] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[3]->addTexture(m_textures[4]);

	m_materials[4] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[4]->addTexture(m_textures[5]);
	m_materials[4]->addTexture(m_textures[14]);
	m_materials[4]->addTexture(m_textures[15]);

	m_materials[5] = GraphicsEngine::get()->createMaterial(m_materials[1]);
	m_materials[5]->addTexture(m_textures[6]);

	m_materials[6] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[6]->addTexture(m_textures[9]); 

	m_materials[7] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\SimpleShader.hlsl");
	m_materials[7]->addTexture(m_textures[11]);
	m_materials[7]->setCullMode(CULL_MODE_BACK);

	m_materials[8] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\StandartShader.hlsl");
	m_materials[8]->addTexture(m_textures[12]); 
	
	m_materials[9] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\SimpleShader.hlsl");
	m_materials[9]->addTexture(m_textures[13]);
	
	m_materials[10] = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\Transparent.hlsl");
	m_materials[10]->addTexture(m_textures[16]);

	material_list.reserve(32);

	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);


	m_materials[10]->addTexture(render_target);
}

void Game::onUpdate()
{
	this->render();
}

void Game::onDestroy()
{
}

void Game::onFocus()
{
}

void Game::onKillFocus()
{
}

void Game::onSize()
{
	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);
}

void Game::onMove()
{
}

void Game::onKeyUp(int key)
{
	if (key == 192)
	{
		show_console = true;
	}
}
