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

		void Bind();

		void UnBind();

		std::vector<Math::Vec3F> GetVertices();

		void SetVertices(const std::vector<Math::Vec3F>& vertices);

		std::vector<Math::Vec3F> GetColors();

		void SetColors(const std::vector<Math::Vec3F>& colors);

		std::vector<unsigned int> GetTriangles();

		void SetTriangles(const std::vector<unsigned int>& triangles);

		std::vector<Math::Vec2F> GetUVs();

		void SetUVs(const std::vector<Math::Vec2F>& uvs);

	private:
		std::vector<Math::Vec3F> vertices;

		std::vector<Math::Vec3F> colors;

		std::vector<unsigned int> triangles;

		std::vector<Math::Vec2F> uvs;

		// IVertexArrayObject
		std::shared_ptr<Graphics::IVertexArrayObject> vertexArrayObject;

		// IVertexBuffer
		std::shared_ptr<Graphics::IVertexBuffer> vertexBuffer;

		// IIndexBuffer
		std::shared_ptr<Graphics::IIndexBuffer> indexBuffer;

		void ValidateMesh();

	};
}
