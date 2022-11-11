#pragma once
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "Prerequisites.h"
#include "Matrix4x4.h"
#include "Rect.h"
#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"

#include "Physics.h"

class Game
{
public:
	void onCreate();
	void onUpdate();
	void onDestroy();
	 void onFocus();
	void onKillFocus();
	void onSize();
	void onMove();

	void onKeyUp(int key);
public:
	void render();
	void updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials);
	void updateCamera();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);

	Rect window_size;
	TexturePtr render_target;
	TexturePtr depth_stencil;
private:
	TexturePtr m_textures[100] = {};
	int number_of_textures = 0;

	MaterialPtr m_materials[100] = {};
	int number_of_materials = 0;

	MeshPtr m_meshes[100] = {};
	int number_of_meshes = 0;

	std::vector<int> physicupdate = {};
	size_t number_of_objects; 
	std::list<GameObject>* items;
	GameObject* cam;

	bool show_console = false;

	float time;
	bool console = false;
	bool fullscreen = false;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;

	std::vector<MaterialPtr> material_list;
private:
	friend class Physics;
	friend class GameObject;
};
