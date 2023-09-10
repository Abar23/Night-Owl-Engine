#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Animation/3D/Structures/BoneWeight.h"
#include "NightOwl/Animation/3D/Structures/BoneInfo.h"
#include "SubMeshData.h"
#include <map>
#include <vector>

namespace NightOwl
{
	class Mesh
	{
	public:
		Mesh();

		void Bind();

		void Unbind();

		std::vector<Vec3F> GetVertices();

		void SetVertices(const std::vector<Vec3F>& vertices);

		std::vector<Vec3F> GetColors();

		void SetColors(const std::vector<Vec3F>& colors);

		void SetColorPerVertex(const Vec3F& color);

		std::vector<Vec3F> GetNormals();

		void SetNormals(const std::vector<Vec3F>& normals);

		std::vector<Vec3F> GetTangents();

		void SetRangents(const std::vector<Vec3F>& tangents);

		std::vector<Vec3F> GetBitangents();

		void SetBitangents(const std::vector<Vec3F>& bitangents);

		std::vector<BoneWeight> GetBoneWeights();

		void SetBoneWeights(const std::vector<BoneWeight>& boneWeights);

		std::vector<Vec3UI> GetTriangles();

		int GetNumberOfTriangles();

		void SetTriangles(const std::vector<Vec3UI>& triangles);

		std::vector<Vec2F> GetUVs();

		void SetUVs(const std::vector<Vec2F>& uvs);

		const std::vector<SubMeshData>& GetSubMeshes();

		const std::map<std::string, BoneInfo>& GetBoneInfoMap();

		void Clear();

		void UploadMeshData();

		REFLECT()

	private:
		std::vector<Vec3F> vertices;

		std::vector<Vec3F> colors;

		std::vector<Vec3F> normals;

		std::vector<Vec3F> tangents;

		std::vector<Vec3F> bitangents;

		std::vector<Vec3UI> triangles;

		std::vector<Vec2F> uvs;

		std::vector<BoneWeight> boneWeights;

		std::shared_ptr<IVertexArrayObject> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<IIndexBuffer> indexBuffer;

		std::vector<SubMeshData> subMeshes;

		std::map<std::string, BoneInfo> boneInfoMap;

		bool isValid;

		void ValidateMesh();

		void UploadVertices();

		void UploadColor();

		void UploadNormals();

		void UploadTangents();

		void UploadBitangents();

		void UploadboneWeights();

		void UploadUvData();

		friend class AssimpModelLoader;
	};
}
