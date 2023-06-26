#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include "NightOwl/Core/Reflection/Reflection.h"
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

		std::vector<Vec3UI> GetTriangles();

		void SetTriangles(const std::vector<Vec3UI>& triangles);

		std::vector<Vec2F> GetUVs();

		void SetUVs(const std::vector<Vec2F>& uvs);

		void Clear();

		void UploadMeshData();

		REFLECT()

	private:
		std::vector<Vec3F> vertices;

		std::vector<Vec3F> colors;

		std::vector<Vec3UI> triangles;

		std::vector<Vec2F> uvs;

		std::shared_ptr<IVertexArrayObject> vertexArrayObject;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<IIndexBuffer> indexBuffer;

		bool isValid;

		void ValidateMesh();

		void UploadVerticesData();

		void UploadColorData();

		void UploadUvData();
	};
}
