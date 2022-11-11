#pragma once
#include <string>
#include "UV.h"

class Console
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
	void UpdateQuad(float x, float y);
	
	Rect window_size;
	TexturePtr render_target;
	TexturePtr depth_stencil;
private:
	std::vector<MaterialPtr> material_list;

	TexturePtr example;
	MaterialPtr example_mat;

	MeshPtr quad;

	float scale_x;
	float scale_y;
	std::string messages[100];

	GameObject items;

	Matrix4x4 m_world_cam;
	Matrix4x4 m_view_cam;
	Matrix4x4 m_proj_cam;
};