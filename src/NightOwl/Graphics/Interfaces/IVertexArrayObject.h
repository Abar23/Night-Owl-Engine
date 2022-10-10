#pragma once

#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include <memory>

namespace NightOwl::Graphics
{
	class IVertexArrayObject
	{
	public:
		virtual ~IVertexArrayObject() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) = 0;

		virtual std::shared_ptr<IVertexBuffer>& GetVertexBuffer() = 0;

		virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) = 0;

		virtual std::shared_ptr<IIndexBuffer>& GetIndexBuffer() = 0;

		virtual unsigned int GetVertexArrayObjectId() = 0;
	};
}
