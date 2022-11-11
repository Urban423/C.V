#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>
#include <filesystem>
#include <iostream>

#include "GraphicsEngine.h"
#include "VertexMesh.h"
#include "Vector3.h"

Mesh::Mesh(const wchar_t* fullpath) : Resource(fullpath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	auto inputfile = std::filesystem::path(fullpath).string();

	std::string mtldit = inputfile.substr(0, inputfile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldit.c_str());

	if (!err.empty()) throw std::exception("Mesh not created successfully");
	if (!res) throw std::exception("Mesh not created successfully");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t size_vertex_index_lists = 0;
	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);

	m_material_slot.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_material_slot[m].start_index = index_global_offset;
		m_material_slot[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m) continue;

				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				Vector3 vertices_face[3];
				Vector2 texcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 2];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 0];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					vertices_face[v] = Vector3(vx, vy, vz);
					texcoords_face[v] = Vector2(tx, ty);
				}

				Vector3 tangent, binormal;
				computeTangents(vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal);

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 2];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 0];

					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					tinyobj::real_t nx = attribs.normals[index.normal_index * 3 + 2];
					tinyobj::real_t ny = attribs.normals[index.normal_index * 3 + 1];
					tinyobj::real_t nz = attribs.normals[index.normal_index * 3 + 0];

					Vector3 v_tangent, v_binormal;
					v_binormal = Vector3::Cross(Vector3(nx, ny, nz), tangent);
					v_tangent = Vector3::Cross(Vector3(nx, ny, nz), v_binormal);

					VertexMesh vertex(Vector3(vx, vy, vz), Vector2(tx, 1 - ty), Vector3(nx, ny,  nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back((unsigned int)index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_material_slot[m].num_indices = index_global_offset - m_material_slot[m].start_index;

	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, (UINT)size_shader);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());

}

Mesh::Mesh(
	VertexMesh* vertex_list_data, unsigned int vertex_list_size, 
	unsigned int* index_list_data, unsigned int index_list_size,
	MaterialSlot* material_slot_list, unsigned int material_slot_list_size) : Resource(L"")
{

	Vector3 tangent, binormal;
	for (int i1 = 0; i1 < index_list_size; i1 += 3)
	{
		computeTangents(vertex_list_data[index_list_data[i1]].position, vertex_list_data[index_list_data[i1 + 1]].position, vertex_list_data[index_list_data[i1 + 2]].position,
			vertex_list_data[index_list_data[i1]].texcoord, vertex_list_data[index_list_data[i1 + 1]].texcoord, vertex_list_data[index_list_data[i1 + 2]].texcoord,
			tangent, binormal);
		
		Vector3 normals = -Vector3::Cross(vertex_list_data[index_list_data[i1 + 2]].position - vertex_list_data[index_list_data[i1 + 1]].position, vertex_list_data[index_list_data[i1]].position - vertex_list_data[index_list_data[i1 + 1]].position).normalized();

		Vector3 v_tangent, v_binormal;
		v_binormal = Vector3::Cross(normals, tangent);
		v_tangent = Vector3::Cross(normals, v_binormal);

		for (int i = 0; i < 3; i ++)
		{
			vertex_list_data[index_list_data[i1 + i]].normal = normals;
			vertex_list_data[index_list_data[i1 + i]].binormal = v_binormal;
			vertex_list_data[index_list_data[i1 + i]].tangent = v_tangent;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list_data, sizeof(VertexMesh), (UINT)vertex_list_size, shader_byte_code, (UINT)size_shader);
	m_index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list_data, (UINT)index_list_size);

	m_material_slot.resize(material_slot_list_size);
	for (unsigned int i = 0; i < material_slot_list_size; i++)
	{
		m_material_slot[i] = material_slot_list[i];
	}
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

const MaterialSlot& Mesh::getMaterialSlot(unsigned int slot)
{
	if (slot >= m_material_slot.size())  MaterialSlot();
	return m_material_slot[slot];
}

size_t Mesh::getNumMaterialSlots()
{
	return m_material_slot.size();
}

void Mesh::computeTangents(
	const Vector3& v0, const Vector3& v1, const Vector3& v2,
	const Vector2& t0, const Vector2& t1, const Vector2& t2,
	Vector3& tangent, Vector3& binormal)
{
	Vector3 deltaPos1 = v1 - v0;
	Vector3 deltaPos2 = v2 - v0;

	Vector2 deltaUV1 = t1 - t0;
	Vector2 deltaUV2 = t2 - t0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y).normalized();
	binormal = (deltaPos2 * deltaUV2.x - deltaPos1 * deltaUV1.x).normalized();
}
