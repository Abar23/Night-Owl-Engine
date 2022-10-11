#pragma once

#include "Component.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include <vector>

namespace NightOwl::Components
{
	class Mesh : public Component
	{
	public:
		Mesh(GameObjects::GameObject* gameObject);

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

	private:
		std::vector<Math::Vec3F> vertices;

		std::vector<Math::Vec3F> colors;

		std::vector<Math::Vec3UI> triangles;

		std::vector<Math::Vec2F> uvs;

		std::shared_ptr<Graphics::IVertexArrayObject> vertexArrayObject;

		std::shared_ptr<Graphics::IVertexBuffer> vertexBuffer;

		std::shared_ptr<Graphics::IIndexBuffer> indexBuffer;

		void ValidateMesh();

	};
}
