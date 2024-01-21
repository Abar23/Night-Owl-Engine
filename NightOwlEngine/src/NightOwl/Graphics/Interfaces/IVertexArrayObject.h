#pragma once

#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include <memory>

namespace NightOwl
{
	class VertexBufferLayout;

	class IGraphicsBuffer;

	class IVertexArrayObject
	{
	public:
		virtual ~IVertexArrayObject() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const std::shared_ptr<IGraphicsBuffer>& vertexBuffer) = 0;

		virtual std::shared_ptr<IGraphicsBuffer>& GetVertexBuffer() = 0;

		virtual void SetIndexBuffer(const std::shared_ptr<IGraphicsBuffer>& indexBuffer) = 0;

		virtual std::shared_ptr<IGraphicsBuffer>& GetIndexBuffer() = 0;

		virtual unsigned int GetVertexArrayObjectId() = 0;

		virtual void SetupVertexBufferAttributes(const VertexBufferLayout& vertexBufferLayout) = 0;

		const VertexBufferLayout* GetVertexBufferLayout() const
		{
			return &vertexBufferLayout;
		}

	protected:
		VertexBufferLayout vertexBufferLayout;
	};
}
