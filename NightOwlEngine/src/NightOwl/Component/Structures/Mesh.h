#pragma once

#include "SubMeshData.h"
#include "NightOwl/Animation/3D/Structures/BoneWeight.h"
#include "NightOwl/Animation/3D/Structures/BoneInfo.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Math/Math.h"
#include <map>
#include <vector>


namespace NightOwl
{
	class IGraphicsBuffer;

	class Mesh
	{
	public:
		Mesh(bool isReadable = true);

		void Bind() const;

		void Unbind() const;

		std::vector<Vec3F> GetVertices();

		void SetVertices(const std::vector<Vec3F>& vertices);

		std::vector<Vec3F> GetColors();

		void SetColors(const std::vector<Vec3F>& colors);

		void SetColorPerVertex(const Vec3F& color);

		std::vector<Vec3F> GetNormals();

		void SetNormals(const std::vector<Vec3F>& normals);

		std::vector<Vec3F> GetTangents();

		void SetTangents(const std::vector<Vec3F>& tangents);

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

		void SetSubMeshes(const std::vector<SubMeshData>& subMeshes);

		const std::map<std::string, BoneInfo>& GetBoneInfoMap();

		void SetBoneInfoMap(const std::map<std::string, BoneInfo>& boneInfoMap);

		void Clear();

		void UploadMeshData(bool markNoLongerReadable);

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

		std::shared_ptr<IGraphicsBuffer> vertexBuffer;

		std::shared_ptr<IGraphicsBuffer> indexBuffer;

		std::vector<SubMeshData> subMeshes;

		std::map<std::string, BoneInfo> boneInfoMap;

		VertexBufferLayout vertexBufferLayout;

		bool isValid;

		bool isReadable;

		void ValidateMesh();

		friend class AssimpModelLoader;
	};
}
