#pragma once

#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"

namespace NightOwl
{
	class OpenGlVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGlVertexBuffer();

		OpenGlVertexBuffer(const void* vertexData, unsigned int vertexDataSize);

		~OpenGlVertexBuffer() override;

		void Bind() override;

		void Unbind() override;

		void SetData(const void* vertexData) override;

		void SetSize(unsigned int vertexDataSize) override;

		void OverwriteVertexBufferDataAtIndex(int index, const void* vertexData, unsigned int vertexDataSize, unsigned customDataSize) override;

		void SetVertexBufferLayout(const VertexBufferLayout& layout) override;

		const VertexBufferLayout& GetVertexBufferLayout() override;

		unsigned GetVertexBufferId() override;

		unsigned GetVertexBufferDataSize() override;

	private:
		unsigned int vertexBufferId;

		unsigned int vertexBufferDataSize;

		VertexBufferLayout layout;
	};
}
