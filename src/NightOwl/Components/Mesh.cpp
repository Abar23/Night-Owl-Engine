#include "Mesh.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/RenderAPI.h"

namespace NightOwl::Components
{
	Mesh::Mesh(GameObjects::GameObject* gameObject)
		: Component(gameObject, ComponentType::Mesh)
	{
		std::vector defaultDataDefinitions = {
			Graphics::VertexBufferData("Position", Graphics::VertexDataType::VectorFloat3),
			Graphics::VertexBufferData("Color", Graphics::VertexDataType::VectorFloat3),
			Graphics::VertexBufferData("UV", Graphics::VertexDataType::VectorFloat2)
		};

		Graphics::VertexBufferLayout defaultLayout(defaultDataDefinitions);

		vertexBuffer = Graphics::RenderApi::CreateVertexBuffer();
		vertexBuffer->SetVertexBufferLayout(defaultLayout);

		indexBuffer = Graphics::RenderApi::CreateIndexBuffer();

		vertexArrayObject = Graphics::RenderApi::CreateVertexArrayObject();
		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);
	}

	void Mesh::Bind()
	{
		vertexArrayObject->Bind();
	}

	void Mesh::Unbind()
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

	std::vector<Math::Vec3UI> Mesh::GetTriangles()
	{
		return triangles;
	}

	void Mesh::SetTriangles(const std::vector<Math::Vec3UI>& triangles)
	{
		this->triangles = triangles;
		indexBuffer->SetSize(triangles.size() * sizeof(Math::Vec3UI));
		indexBuffer->SetData(triangles.data());
		vertexBuffer->SetSize(vertexBuffer->GetVertexBufferLayout().GetDataPerTriangle() * triangles.size() * sizeof(Math::Vec3UI));
		vertexBuffer->OverwriteVertexBufferDataAtIndex(2, uvs.data(), Graphics::VertexDataTypeToDataTypeSize(Graphics::VertexDataType::VectorFloat2) * uvs.size());
		vertexBuffer->OverwriteVertexBufferDataAtIndex(1, colors.data(), Graphics::VertexDataTypeToDataTypeSize(Graphics::VertexDataType::VectorFloat3) * colors.size());
		vertexBuffer->OverwriteVertexBufferDataAtIndex(0, vertices.data(), Graphics::VertexDataTypeToDataTypeSize(Graphics::VertexDataType::VectorFloat3) * vertices.size());
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
