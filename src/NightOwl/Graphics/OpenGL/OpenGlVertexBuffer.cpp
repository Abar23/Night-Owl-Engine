#include "OpenGlVertexBuffer.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Assert.h"

namespace NightOwl::Graphics
{
	OpenGlVertexBuffer::OpenGlVertexBuffer()
	{
		GL_CALL(glCreateBuffers, 1, &vertexBufferId);
	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(const void* vertexData, unsigned int vertexDataSize)
	{
		this->vertexBufferDataSize = vertexDataSize;
		GL_CALL(glCreateBuffers, 1, &vertexBufferId);
		GL_CALL(glNamedBufferData, vertexBufferId, vertexDataSize, vertexData, GL_DYNAMIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &vertexBufferId);
	}

	void OpenGlVertexBuffer::Bind()
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, vertexBufferId);
	}

	void OpenGlVertexBuffer::Unbind()
	{
		GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::SetData(const void* vertexData)
	{
		GL_CALL(glNamedBufferSubData, vertexBufferId, 0, vertexBufferDataSize, vertexData);
	}

	void OpenGlVertexBuffer::SetSize(unsigned int vertexDataSize)
	{
		vertexBufferDataSize = vertexDataSize;
		GL_CALL(glNamedBufferData, vertexBufferId, vertexDataSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void Graphics::OpenGlVertexBuffer::OverwriteVertexBufferDataAtIndex(int index, const void* vertexData, unsigned int vertexDataSize)
	{
		if(vertexDataSize != 0)
		{
			ENGINE_ASSERT(index < layout.GetBufferDataDefinitions().size(), std::format("Vertex buffer does not contain a vertex buffer data definition at index {0}", index));

			const VertexBufferData& data = layout.GetBufferDataDefinitions()[index];

			unsigned int offset = 0;
			for (int i = 0; i < index; i++)
			{
				offset += layout.GetBufferDataDefinitions()[i].GetSizeofData();
			}

			const char* byteVertexDataPointer = static_cast<const char*>(vertexData);
			const char* byteVertexDataPointerEnd = byteVertexDataPointer + vertexDataSize;
			while (byteVertexDataPointer < byteVertexDataPointerEnd)
			{
				GL_CALL(glNamedBufferSubData, vertexBufferId, offset, data.GetSizeofData(), byteVertexDataPointer);
				byteVertexDataPointer += data.GetSizeofData();
				offset += layout.GetDataPerVertex();
			}
		}
	}

	void OpenGlVertexBuffer::SetVertexBufferLayout(const VertexBufferLayout& layout)
	{
		this->layout = layout;
	}

	const VertexBufferLayout& OpenGlVertexBuffer::GetVertexBufferLayout()
	{
		return layout;
	}

	unsigned OpenGlVertexBuffer::GetVertexBufferId()
	{
		return vertexBufferId;
	}

	unsigned OpenGlVertexBuffer::GetVertexBufferDataSize()
	{
		return vertexBufferDataSize;
	}
}
