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

class UV
{
public:
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();
	virtual void onMove();

	void render();
	void updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials);
	void updateCamera();
	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials);

	Rect window_size;
	TexturePtr render_target;
	TexturePtr depth_stencil;
private:
	std::vector<MaterialPtr> material_list;

	TexturePtr example;
	MaterialPtr example_mat;

	MeshPtr quad;

	GameObject items;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;
};