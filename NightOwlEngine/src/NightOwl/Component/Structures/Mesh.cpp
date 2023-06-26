#include <NightOwlPch.h>

#include "Mesh.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/RenderAPI.h"

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

		if (isValid)
		{
			UploadVerticesData();
		}
	}

	std::vector<Vec3F> Mesh::GetColors()
	{
		return colors;
	}

	void Mesh::SetColors(const std::vector<Vec3F>& colors)
	{
		this->colors = colors;

		if (isValid)
		{
			UploadColorData();
		}
	}

	void Mesh::SetColorPerVertex(const Vec3F& color)
	{
		const std::vector<Vec3F> colors(4, color / 255.0f);

		this->SetColors(colors);
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

		UploadMeshData();
	}

	std::vector<Vec2F> Mesh::GetUVs()
	{
		return uvs;
	}

	void Mesh::SetUVs(const std::vector<Vec2F>& uvs)
	{
		this->uvs = uvs;

		if (isValid)
		{
			UploadUvData();
		}
	}

	void Mesh::Clear()
	{
		triangles.clear();
		vertices.clear();
		uvs.clear();
		colors.clear();

		isValid = false;
	}

	void Mesh::UploadMeshData()
	{
		ENGINE_ASSERT(isValid, "Triangle indices reference out of bounds vertices.");

		indexBuffer->SetSize(triangles.size() * sizeof(Vec3UI));
		indexBuffer->SetData(triangles.data());

		VertexBufferLayout layout;

		VertexBufferData data = VertexBufferData("Position", VertexDataType::VectorFloat3, 0);
		layout.AddVertexBufferDataDefinition(data);

		if (!colors.empty())
		{
			data = VertexBufferData("Color", VertexDataType::VectorFloat3, 1);
			layout.AddVertexBufferDataDefinition(data);
		}
		if (!uvs.empty())
		{
			data = VertexBufferData("UV", VertexDataType::VectorFloat2, 2);
			layout.AddVertexBufferDataDefinition(data);
		}

		vertexBuffer->SetVertexBufferLayout(layout);
		vertexBuffer->SetSize(layout.GetDataPerVertex() * triangles.size() * 3);

		int indexOfVertexBufferData = 0;

		vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, vertices.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * vertices.size());
		indexOfVertexBufferData++;

		if (!colors.empty())
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, colors.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * colors.size());
			indexOfVertexBufferData++;
		}
		if (!uvs.empty())
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, uvs.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat2) * uvs.size());
		}

		vertexArrayObject->SetupVertexBufferAttributes();
	}

	void Mesh::ValidateMesh()
	{
		isValid = false;

		const int verticesPerTriangle = VertexDataTypeToNumberOfComponents(VertexDataType::VectorFloat3);

		const auto expectedNumberOfVerticesPerTriangle = verticesPerTriangle * triangles.size();
		const auto actualNumberOfVerticesPerTriangle = verticesPerTriangle * vertices.size();

		if (expectedNumberOfVerticesPerTriangle <= actualNumberOfVerticesPerTriangle)
		{
			isValid = true;
		}
	}

	void Mesh::UploadVerticesData()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Position");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, vertices.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * vertices.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadColorData()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Color");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, colors.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * colors.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadUvData()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("UV");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, uvs.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat2) * uvs.size());
			return;
		}

		UploadMeshData();
	}

	START_REFLECTION(Mesh)
		CLASS_MEMBER_REFLECTION(vertices)
		CLASS_MEMBER_REFLECTION(colors)
		CLASS_MEMBER_REFLECTION(triangles)
		CLASS_MEMBER_REFLECTION(uvs)
		END_REFLECTION(Mesh)
}
