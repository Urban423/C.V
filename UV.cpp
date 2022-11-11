#include "UV.h"


__declspec(align(16))
struct constante
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_camera_position;
};

void UV::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->render_target, 0, 0 , 0, 0);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(this->render_target, this->depth_stencil);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(window_size.width, window_size.height);


	material_list.clear();
	material_list.push_back(example_mat);
	updateModel(&items, material_list);
	//drawMesh(quad, material_list);

}

void UV::updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials)
{
	constante cc;

	obj->processWorldMatrix();

	cc.m_world = obj->GetWorldMatrix();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();


	for (int i = 0; i < materials.size(); i++)
	{
		materials[i]->setData(&cc, sizeof(constante));
	}
}

void UV::updateCamera()
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

	m_proj_cam.setPersepciveFovLH(1, ((float)window_size.width / (float)window_size.height), 0.03f, 1000);
}

void UV::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
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

void UV::onCreate()
{
	example = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\Point.png");

	float scale = 0.013f;

	float right = 0;

	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3(-scale + right, -scale, 0), Vector2(0, 1), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(-scale + right,  scale, 0), Vector2(0, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3( scale + right,  scale, 0), Vector2(1, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3( scale + right, -scale, 0), Vector2(1, 1), Vector3(),  Vector3(),  Vector3())
	};

	unsigned int quad_index_list[] = {
		0, 1, 2,
		2, 3, 0
	};

	MaterialSlot quad_mat_slots[] = { {0, 6, 0} };

	quad = GraphicsEngine::get()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);

	example_mat = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\UVShader.hlsl");
	example_mat->setCullMode(CULL_MODE::CULL_MODE_BACK);
	example_mat->addTexture(example);

	material_list.reserve(32);

	updateCamera();

	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);
}

void UV::onUpdate()
{
	render();
}

void UV::onDestroy()
{
}

void UV::onFocus()
{
}

void UV::onKillFocus()
{
}

void UV::onSize()
{
	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);
	updateCamera();
}

void UV::onMove()
{

}