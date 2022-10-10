#pragma once

#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"

namespace NightOwl::Graphics
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetData(const void* vertexData, unsigned int vertexDataSize) = 0;

		virtual void SetVertexBufferLayout(const VertexBufferLayout& layout) = 0;

		virtual const VertexBufferLayout& GetVertexBufferLayout() = 0;

		virtual unsigned int GetVertexBufferId() = 0;

		virtual unsigned int GetVertexBufferDataSize() = 0;
	};
}
