#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include <vector>

namespace NightOwl::Components
{
	class Mesh
	{
	public:
		Mesh();

		std::vector<Math::Vec3F> GetVertices();

		void SetVertices(const std::vector<Math::Vec3F>& vertices);

		//std::vector<Math::Vec3F> GetNormals();

		//void SetNormals(const std::vector<Math::Vec3F>& vertices);

		//std::vector<Math::Vec3F> GetTangents();

		//void SetTangents(const std::vector<Math::Vec3F>& vertices);

		std::vector<Math::Vec3F> GetColors();

		void SetColors(const std::vector<Math::Vec3F>& colors);

		std::vector<Math::Vec3F> GetTriangles();

		void SetTriangles(const std::vector<Math::Vec3F>& triangles);

		std::vector<Math::Vec2F> GetUVs();

		void SetUVs(const std::vector<Math::Vec3F>& uvs);

	private:
		std::vector<Math::Vec3F> vertices;

		//std::vector<Math::Vec3F> normals;

		//std::vector<Math::Vec3F> tangents;

		std::vector<Math::Vec3F> colors;

		std::vector<Math::Vec3F> triangles;

		std::vector<Math::Vec2F> uv;

		// IVertexArrayObject
		std::shared_ptr<Graphics::IVertexArrayObject> vertexArrayObject;

		// IVertexBuffer
		std::shared_ptr<Graphics::IVertexBuffer> vertexBuffer;

		// IIndexBuffer
		std::shared_ptr<Graphics::IIndexBuffer> indexBuffer;

	};
}
