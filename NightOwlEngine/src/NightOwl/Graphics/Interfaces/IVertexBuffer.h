#pragma once

#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"

namespace NightOwl
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetSize(unsigned int vertexDataSize) = 0;

		virtual void SetData(const void* vertexData) = 0;

		virtual void OverwriteVertexBufferDataAtIndex(int index, const void* vertexData, unsigned int vertexDataSize, unsigned customDataSize = 0) = 0;

		virtual void SetVertexBufferLayout(const VertexBufferLayout& layout) = 0;

		virtual const VertexBufferLayout& GetVertexBufferLayout() = 0;

		virtual unsigned int GetVertexBufferId() = 0;

		virtual unsigned int GetVertexBufferDataSize() = 0;
	};
}
