#include <NightOwlPch.h>

#include "Mesh.h"
#include "NightOwl/Graphics/RenderAPI.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"

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

		if (isValid)
		{
			UploadVertices();
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
			UploadColor();
		}
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

		if (!normals.empty())
		{
			data = VertexBufferData("Normals", VertexDataType::VectorFloat3, 3);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!normals.empty())
		{
			data = VertexBufferData("Tangents", VertexDataType::VectorFloat3, 4);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!normals.empty())
		{
			data = VertexBufferData("Bitangents", VertexDataType::VectorFloat3, 5);
			layout.AddVertexBufferDataDefinition(data);
		}

		if (!boneWeights.empty())
		{
			data = VertexBufferData("BoneIds", VertexDataType::VectorInt4, 6);
			layout.AddVertexBufferDataDefinition(data);
		
			data = VertexBufferData("BoneWeights", VertexDataType::VectorFloat4, 7);
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
			indexOfVertexBufferData++;
		}
		if (!normals.empty())
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, normals.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * normals.size());
			indexOfVertexBufferData++;
		}
		if (!tangents.empty())
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, tangents.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * tangents.size());
			indexOfVertexBufferData++;
		}
		if (!bitangents.empty())
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, bitangents.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * bitangents.size());
			indexOfVertexBufferData++;
		}

		// TODO: Does work, but should be cleaner
		if (!boneWeights.empty())
		{
			unsigned int boneWeightsVertexDataSize = VertexDataTypeToDataTypeSize(VertexDataType::VectorInt4) + VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat4);
			vertexBuffer->OverwriteVertexBufferDataAtIndex(indexOfVertexBufferData, boneWeights.data(), boneWeightsVertexDataSize * boneWeights.size(), boneWeightsVertexDataSize);
		}

		vertexArrayObject->SetupVertexBufferAttributes();
	}

	void Mesh::ValidateMesh()
	{
		isValid = false;

		if (triangles.empty() == false && vertices.empty() == false)
		{
			isValid = true;
		}
	}

	void Mesh::UploadVertices()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Position");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, vertices.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * vertices.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadColor()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Color");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, colors.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * colors.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadNormals()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Normals");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, normals.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * normals.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadTangents()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Tangents");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, tangents.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * tangents.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadBitangents()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("Bitangents");
		if (index >= 0)
		{
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, bitangents.data(), VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat3) * bitangents.size());
			return;
		}

		UploadMeshData();
	}

	void Mesh::UploadBoneWeights()
	{
		const int index = vertexBuffer->GetVertexBufferLayout().GetIndexOfShaderAttribute("BoneIds");
		if (index >= 0)
		{
			unsigned int boneWeightsVertexDataSize = VertexDataTypeToDataTypeSize(VertexDataType::VectorInt4) + VertexDataTypeToDataTypeSize(VertexDataType::VectorFloat4);
			vertexBuffer->OverwriteVertexBufferDataAtIndex(index, boneWeights.data(), boneWeightsVertexDataSize * boneWeights.size());
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
