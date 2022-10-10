#pragma once

#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"

namespace NightOwl::Graphics
{
	class OpenGlVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGlVertexBuffer() = delete;

		OpenGlVertexBuffer(const void* vertexData, unsigned int vertexDataSize);

		~OpenGlVertexBuffer() override;

		void Bind() override;

		void Unbind() override;

		void SetData(const void* vertexData, unsigned vertexDataSize) override;

		void SetVertexBufferLayout(const VertexBufferLayout& layout) override;

		const VertexBufferLayout& GetVertexBufferLayout() override;

		unsigned GetVertexBufferId() override;

		unsigned GetVertexBufferDataSize() override;

	private:
		unsigned int vertexBufferId;

		unsigned int vertexDataSize;

		VertexBufferLayout layout;
	};
}
