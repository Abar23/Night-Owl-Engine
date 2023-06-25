#include "NightOwlPch.h"

#include "Mesh.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Component/Types/ComponentType.h"

namespace NightOwl
{
	Mesh::Mesh()
		: isValid(false)
	{
		vertexBuffer = RenderApi::CreateVertexBuffer();

		indexBuffer = RenderApi::CreateIndexBuffer();

		vertexArrayObject = RenderApi::CreateVertexArrayObject();
		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);
	}

	void Mesh::Bind()
	{
		ENGINE_ASSERT(isValid, "Mesh with invalid data has been bound!"); // get name to be apart of mesh for better debugging

		vertexArrayObject->Bind();
	}

	void Mesh::Unbind()
	{
		vertexArrayObject->Unbind();
	}

	std::vector<Vec3F> Mesh::GetVertices()
	{
		return vertices;
	}

	void Mesh::SetVertices(const std::vector<Vec3F>& vertices)
	{
		this->vertices = vertices;
	}

	std::vector<Vec3F> Mesh::GetColors()
	{
		return colors;
	}

	void Mesh::SetColors(const std::vector<Vec3F>& colors)
	{
		this->colors = colors;
	}

	std::vector<Vec3UI> Mesh::GetTriangles()
	{
		return triangles;
	}

	void Mesh::SetTriangles(const std::vector<Vec3UI>& triangles)
	{
		ENGINE_ASSERT(!triangles.empty(), "Must call clear data before reassigning triangles of mesh.");

		this->triangles = triangles;

		ValidateMesh();

		ENGINE_ASSERT(isValid, "Triangle indices reference out of bounds vertices.");

		indexBuffer->SetSize(triangles.size() * sizeof(Vec3UI));
		indexBuffer->SetData(triangles.data());

		VertexBufferLayout layout;
		layout.AddVertexBufferDataDefinition(VertexBufferData("Position", VertexDataType::VectorFloat3, 0));
		if(!colors.empty())
		{
			layout.AddVertexBufferDataDefinition(VertexBufferData("Color", VertexDataType::VectorFloat3, 1));
		}
		if(!uvs.empty())
		{
			layout.AddVertexBufferDataDefinition(VertexBufferData("UV", VertexDataType::VectorFloat2, 2));
		}

		vertexBuffer->SetVertexBufferLayout(layout);
		vertexBuffer->SetSize(layout.GetDataPerVertex() * triangles.size() * 3);

		int attributeVertexBufferLayoutIndex = 0;
		vertexBuffer->OverwriteVertexBufferDataAtIndex(attributeVertexBufferLayoutIndex, vertices.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * vertices.size());
		vertexBuffer->OverwriteVertexBufferDataAtIndex(attributeVertexBufferLayoutIndex, colors.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * colors.size());
		vertexBuffer->OverwriteVertexBufferDataAtIndex(attributeVertexBufferLayoutIndex, uvs.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat2) * uvs.size());

		vertexArrayObject->SetupVertexBufferAttributes();
	}

	std::vector<Vec2F> Mesh::GetUVs()
	{
		return uvs;
	}

	void Mesh::SetUVs(const std::vector<Vec2F>& uvs)
	{
		this->uvs = uvs;
	}

	void Mesh::Clear()
	{
		triangles.clear();
		vertices.clear();
		uvs.clear();
		colors.clear();

		isValid = false;
	}

	void Mesh::ValidateMesh()
	{
		isValid = false;

		const auto expectedNumberOfVerticesPerTriangle = 3 * triangles.size();

		if(expectedNumberOfVerticesPerTriangle <= vertices.size() * VertexDataTypeToNumberOfComponents(VertexDataType::VectorFloat3))
		{
			isValid = true;
		}
	}

	START_REFLECTION(Mesh)
	CLASS_MEMBER_REFLECTION(vertices)
	CLASS_MEMBER_REFLECTION(colors)
	CLASS_MEMBER_REFLECTION(triangles)
	CLASS_MEMBER_REFLECTION(uvs)
	END_REFLECTION(Mesh)
}
