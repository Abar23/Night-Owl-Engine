#include <NightOwlPch.h>

#include "Mesh.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/Types/VertexDataTypes.h"

namespace NightOwl
{
	Mesh::Mesh(bool isReadable /* = true */)
		: isValid(false),
		  isReadable(isReadable)
	{
		vertexBuffer = Graphics::CreateGraphicsBuffer(BufferType::Vertex);
		indexBuffer = Graphics::CreateGraphicsBuffer(BufferType::Index);

		vertexArrayObject = Graphics::CreateVertexArrayObject();
		vertexArrayObject->SetIndexBuffer(indexBuffer);
		vertexArrayObject->SetVertexBuffer(vertexBuffer);
	}

	void Mesh::Bind() const
	{
		ENGINE_ASSERT(isValid, "Mesh with invalid data has been bound!"); // get name to be apart of mesh for better debugging

		vertexArrayObject->Bind();
	}

	void Mesh::Unbind() const
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

	// TODO: Don't like the divide by 255
	void Mesh::SetColorPerVertex(const Vec3F& color)
	{
		const std::vector<Vec3F> colors(vertices.size(), color / 255.0f);

		this->SetColors(colors);
	}

	std::vector<Vec3F> Mesh::GetNormals()
	{
		return normals;
	}

	void Mesh::SetNormals(const std::vector<Vec3F>& normals)
	{
		this->normals = normals;
	}

	std::vector<Vec3F> Mesh::GetTangents()
	{
		return tangents;
	}

	void Mesh::SetTangents(const std::vector<Vec3F>& tangents)
	{
		this->tangents = tangents;
	}

	std::vector<Vec3F> Mesh::GetBitangents()
	{
		return bitangents;
	}

	void Mesh::SetBitangents(const std::vector<Vec3F>& bitangents)
	{
		this->bitangents = bitangents;
	}

	std::vector<BoneWeight> Mesh::GetBoneWeights()
	{
		return boneWeights;
	}

	void Mesh::SetBoneWeights(const std::vector<BoneWeight>& boneWeights)
	{
		this->boneWeights = boneWeights;
	}

	std::vector<Vec3UI> Mesh::GetTriangles()
	{
		return triangles;
	}

	int Mesh::GetNumberOfTriangles()
	{
		return triangles.size();
	}

	void Mesh::SetTriangles(const std::vector<Vec3UI>& triangles)
	{
		ENGINE_ASSERT(!triangles.empty(), "Must call clear data before reassigning triangles of mesh.");

		this->triangles = triangles;
	}

	std::vector<Vec2F> Mesh::GetUVs()
	{
		return uvs;
	}

	void Mesh::SetUVs(const std::vector<Vec2F>& uvs)
	{
		this->uvs = uvs;
	}

	const std::vector<SubMeshData>& Mesh::GetSubMeshes()
	{
		return subMeshes;
	}

	void Mesh::SetSubMeshes(const std::vector<SubMeshData>& subMeshes)
	{
		this->subMeshes = subMeshes;
	}

	const std::map<std::string, BoneInfo>& Mesh::GetBoneInfoMap()
	{
		return boneInfoMap;
	}

	void Mesh::SetBoneInfoMap(const std::map<std::string, BoneInfo>& boneInfoMap)
	{
		this->boneInfoMap = boneInfoMap;
	}

	void Mesh::Clear()
	{
		triangles.clear();
		vertices.clear();
		uvs.clear();
		normals.clear();
		tangents.clear();
		bitangents.clear();
		colors.clear();
		boneWeights.clear();
	}

	void Mesh::UploadMeshData(bool markNoLongerReadable)
	{
		ValidateMesh();
		ENGINE_ASSERT(isValid, "Triangle indices reference out of bounds vertices.");

		indexBuffer->SetSize(triangles.size(), sizeof(triangles[0]));
		indexBuffer->SetData(triangles.data());
		
		VertexBufferLayout layout;

		VertexBufferData data = VertexBufferData(VertexDataType::Position, 0);
		layout.AddVertexBufferDataDefinition(data);

		if (!colors.empty())
		{
			data = VertexBufferData(VertexDataType::Color, 1);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!uvs.empty())
		{
			data = VertexBufferData(VertexDataType::Uv0, 2);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!normals.empty())
		{
			data = VertexBufferData(VertexDataType::Normal, 3);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!tangents.empty())
		{
			data = VertexBufferData(VertexDataType::Tangent, 4);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!bitangents.empty())
		{
			data = VertexBufferData(VertexDataType::Bitangent, 5);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!boneWeights.empty())
		{
			data = VertexBufferData(VertexDataType::BoneWeights, 6);
			layout.AddVertexBufferDataDefinition(data);
		}

		vertexBufferLayout = layout;
		vertexBuffer->SetSize(vertices.size(), layout.GetDataPerVertex());

		const VertexBufferData* vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Position);
		vertexBuffer->SetSubData(vertices.data(), 0, vertices.size(), vertexBufferData->GetSizeofData());

		if (!colors.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Color);
			vertexBuffer->SetSubData(colors.data(), vertexBufferData->GetOffset(), colors.size(), vertexBufferData->GetSizeofData());
		}
		if (!uvs.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Uv0);
			vertexBuffer->SetSubData(uvs.data(), vertexBufferData->GetOffset(), uvs.size(), vertexBufferData->GetSizeofData());
		}
		if (!normals.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Normal);
			vertexBuffer->SetSubData(normals.data(), vertexBufferData->GetOffset(), normals.size(), vertexBufferData->GetSizeofData());
		}
		if (!tangents.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Tangent);
			vertexBuffer->SetSubData(tangents.data(), vertexBufferData->GetOffset(), tangents.size(), vertexBufferData->GetSizeofData());
		}
		if (!bitangents.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::Bitangent);
			vertexBuffer->SetSubData(bitangents.data(), vertexBufferData->GetOffset(), bitangents.size(), vertexBufferData->GetSizeofData());
		}
		if (!boneWeights.empty())
		{
			vertexBufferData = layout.GetVertexBufferDataForVertexDataType(VertexDataType::BoneWeights);
			vertexBuffer->SetSubData(boneWeights.data(), vertexBufferData->GetOffset(), boneWeights.size(), vertexBufferData->GetSizeofData());
		}

		vertexArrayObject->SetupVertexBufferAttributes(vertexBufferLayout);

		if (markNoLongerReadable)
		{
			Clear();
		}
	}

	void Mesh::ValidateMesh()
	{
		// Find greatest index in the triangles array
		unsigned int maxIndexInsideTriangles = 0;
		for (const Vec3UI& triangle : triangles)
		{
			maxIndexInsideTriangles = std::max(maxIndexInsideTriangles, triangle.x);
			maxIndexInsideTriangles = std::max(maxIndexInsideTriangles, triangle.y);
			maxIndexInsideTriangles = std::max(maxIndexInsideTriangles, triangle.z);
		}

		isValid = true;
		if (vertices.empty() && vertices.size() < maxIndexInsideTriangles)
		{
			ENGINE_LOG_ASSERT("The supplied vertex array has less vertices than referenced in the triangles array.");
			isValid = false;
		}

		if (normals.empty() == false && normals.size() != vertices.size())
		{
			ENGINE_LOG_ASSERT("The supplied normals array has to be the same size as the vertices array");
			isValid = false;
		}

		if (tangents.empty() == false && tangents.size() != vertices.size())
		{
			ENGINE_LOG_ASSERT("The supplied tangents array has to be the same size as the vertices array");
			isValid = false;
		}

		if (bitangents.empty() == false && bitangents.size() != vertices.size())
		{
			ENGINE_LOG_ASSERT("The supplied bitangents array has to be the same size as the vertices array");
			isValid = false;
		}

		if (uvs.empty() == false && uvs.size() != vertices.size())
		{
			ENGINE_LOG_ASSERT("The supplied uvs array has to be the same size as the vertices array");
			isValid = false;
		}

		if (boneWeights.empty() == false && boneWeights.size() != vertices.size())
		{
			ENGINE_LOG_ASSERT("The supplied boneWeights array has to be the same size as the vertices array");
			isValid = false;
		}
	}

	START_REFLECTION(Mesh)
	CLASS_MEMBER_REFLECTION(vertices)
	CLASS_MEMBER_REFLECTION(colors)
	CLASS_MEMBER_REFLECTION(triangles)
	CLASS_MEMBER_REFLECTION(uvs)
	END_REFLECTION(Mesh)
}
