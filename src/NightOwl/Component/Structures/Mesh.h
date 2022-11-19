#pragma once

#include "NightOwl/Math/Math.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include <vector>

namespace NightOwl::Component
{
	class Mesh
	{
	public:
		Mesh();

		void Bind();

		void Unbind();

		std::vector<Math::Vec3F> GetVertices();

		void SetVertices(const std::vector<Math::Vec3F>& vertices);

		std::vector<Math::Vec3F> GetColors();

		void SetColors(const std::vector<Math::Vec3F>& colors);

		std::vector<Math::Vec3UI> GetTriangles();

		void SetTriangles(const std::vector<Math::Vec3UI>& triangles);

		std::vector<Math::Vec2F> GetUVs();

		void SetUVs(const std::vector<Math::Vec2F>& uvs);

		void Clear();

		REFLECT()

	private:
		std::vector<Math::Vec3F> vertices;

		std::vector<Math::Vec3F> colors;

		std::vector<Math::Vec3UI> triangles;

		std::vector<Math::Vec2F> uvs;

		std::shared_ptr<Graphics::IVertexArrayObject> vertexArrayObject;

		std::shared_ptr<Graphics::IVertexBuffer> vertexBuffer;

		std::shared_ptr<Graphics::IIndexBuffer> indexBuffer;

		bool isValid;

		void ValidateMesh();

	};
}
