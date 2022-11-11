#include "Console.h"

__declspec(align(16))
struct constante
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	Vector4 m_camera_position;
	Vector2 pos;
	Vector2 letter;
};

void Console::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->render_target, 0, 0, 0, 0);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearDepthStencil(this->depth_stencil);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setRenderTarget(this->render_target, this->depth_stencil);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(window_size.width, window_size.height);

	material_list.clear();
	material_list.push_back(example_mat);
	items.transform.position.x = -window_size.width / 2 + scale_x;
	items.transform.position.y = window_size.height / 2 - scale_y;

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < messages[i].size(); j++)
		{
			updateModel(&items, material_list);
			drawMesh(quad, material_list); 
			items.transform.position.x += scale_x * 2;
			if (messages[i][j] == '\n')
			{
				items.transform.position.x = -window_size.width / 2 + scale_x;
				items.transform.position.y -= scale_y * 2;
			}
		};
		items.transform.position.x = -window_size.width / 2 + scale_x;
		items.transform.position.y -= scale_y * 2;
	}

}

void Console::updateModel(GameObject* obj, const std::vector<MaterialPtr>& materials)
{
	constante cc;

	obj->processWorldMatrix();

	cc.m_world = obj->GetWorldMatrix();
	cc.m_view = m_view_cam;
	cc.m_proj = m_proj_cam;
	cc.m_camera_position = m_world_cam.getTranslation();
	cc.pos = Vector2(obj->transform.position.x, obj->transform.position.y);
	cc.letter = Vector2(3, 4);

	for (int i = 0; i < materials.size(); i++)
	{
		materials[i]->setData(&cc, sizeof(constante));
	}
}

void Console::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	Quaternion t = Quaternion(Vector3(0, 0, 0));
	temp.QuaternionToMatrix(&t);
	world_cam *= temp;

	world_cam.setTranslation(Vector3(0, 0, -12));
	m_world_cam = world_cam;

	world_cam.inverse();
	m_view_cam = world_cam;

	m_proj_cam.setOrthoLH((float)window_size.width, (float)window_size.height, 0.03f, 1000);
}

void Console::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& materials)
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

void Console::UpdateQuad(float x, float y)
{
	VertexMesh quad_vertex_list[] =
	{
		VertexMesh(Vector3(-x, -y, 0), Vector2(0, 1), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(-x, y, 0), Vector2(0, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(x, y, 0), Vector2(1, 0), Vector3(),  Vector3(),  Vector3()),
		VertexMesh(Vector3(x, -y, 0), Vector2(1, 1), Vector3(),  Vector3(),  Vector3())
	};

	unsigned int quad_index_list[] = {
		0, 1, 2,
		2, 3, 0
	};

	MaterialSlot quad_mat_slots[] = { {0, 6, 0} };

	quad = GraphicsEngine::get()->getMeshManager()->createMesh(quad_vertex_list, 4, quad_index_list, 6, quad_mat_slots, 1);
}

void Console::onCreate()
{
	example = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets\\Textures\\shrift.png");

	scale_y = (float)window_size.height / 50;
	scale_x = scale_y;
	UpdateQuad(scale_x, scale_y);

	example_mat = GraphicsEngine::get()->createMaterialFromFile(L"Assets\\Shaders\\UVShader.hlsl");
	example_mat->setCullMode(CULL_MODE::CULL_MODE_BACK);
	example_mat->addTexture(example);

	material_list.reserve(32);

	updateCamera();

	messages[0] = "apple";
	messages[1] = "i \n ate apple";

	messages[3] = "44";

	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);
}

void Console::onUpdate()
{
	render();
}

void Console::onDestroy()
{
}

void Console::onFocus()
{
}

void Console::onKillFocus()
{
}

void Console::onSize()
{
	render_target = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::RenderTarget);
	depth_stencil = GraphicsEngine::get()->getTextureManager()->createTexture(Rect(window_size.width, window_size.height), Texture::Type::DepthStencil);
	updateCamera();


	scale_y = (float)window_size.height / 50;
	scale_x = scale_y;
	UpdateQuad(scale_x, scale_y);
}

void Console::onMove()
{

}