#include "Mesh.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/RenderAPI.h"

namespace NightOwl::Components
{
	Mesh::Mesh()
	{
		std::vector defaultDataDefinitions = {
			Graphics::VertexBufferData("Position", Graphics::VertexDataType::VectorFloat3),
			Graphics::VertexBufferData("Position", Graphics::VertexDataType::VectorFloat3),
			Graphics::VertexBufferData("Position", Graphics::VertexDataType::VectorFloat2)
		};

		Graphics::VertexBufferLayout defaultLayout(defaultDataDefinitions);

		vertexBuffer = Graphics::RenderApi::CreateVertexBuffer();
		vertexBuffer->SetVertexBufferLayout(defaultLayout);
		indexBuffer = Graphics::RenderApi::CreateIndexBuffer();
		vertexArrayObject = Graphics::RenderApi::CreateVertexArrayObject();
	}

	void Mesh::Bind()
	{
		vertexArrayObject->Bind();
	}

	void Mesh::UnBind()
	{
		vertexArrayObject->Unbind();
	}

	std::vector<Math::Vec3F> Mesh::GetVertices()
	{
		return vertices;
	}

	void Mesh::SetVertices(const std::vector<Math::Vec3F>& vertices)
	{
		this->vertices = vertices;
	}

	std::vector<Math::Vec3F> Mesh::GetColors()
	{
		return colors;
	}

	void Mesh::SetColors(const std::vector<Math::Vec3F>& colors)
	{
		this->colors = colors;
	}

	std::vector<unsigned> Mesh::GetTriangles()
	{
		return triangles;
	}

	void Mesh::SetTriangles(const std::vector<unsigned>& triangles)
	{
		this->triangles = triangles;
	}

	std::vector<Math::Vec2F> Mesh::GetUVs()
	{
		return uvs;
	}

	void Mesh::SetUVs(const std::vector<Math::Vec2F>& uvs)
	{
		this->uvs = uvs;
	}

	void Mesh::ValidateMesh()
	{

	}
}
