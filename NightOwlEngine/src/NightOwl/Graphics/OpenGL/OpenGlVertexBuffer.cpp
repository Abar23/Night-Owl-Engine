#include <NightOwlPch.h>

#include "OpenGlVertexBuffer.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl
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

	void OpenGlVertexBuffer::OverwriteVertexBufferDataAtIndex(int index, const void* vertexData, unsigned int vertexDataSize, unsigned customDataSize)
	{
		ENGINE_ASSERT(index < layout.GetBufferDataDefinitions().size(), "Vertex buffer does not contain a vertex buffer data definition at index {0}", index);

		if(vertexDataSize == 0)
		{
			return;
		}

		if(index < 0)
		{
			return;
		}

		const VertexBufferData& data = layout.GetBufferDataDefinitions()[index];

		// TODO: Think of better method than custom data size
		unsigned int dataSize = customDataSize > 0 ? customDataSize : data.GetSizeofData();

		unsigned int offset = data.GetOffset();

		const char* byteVertexDataPointerBegin = static_cast<const char*>(vertexData);
		const char* byteVertexDataPointerEnd = byteVertexDataPointerBegin + vertexDataSize;

		while (byteVertexDataPointerBegin < byteVertexDataPointerEnd)
		{
			GL_CALL(glNamedBufferSubData, vertexBufferId, offset, dataSize, byteVertexDataPointerBegin);
			byteVertexDataPointerBegin += dataSize;
			offset += layout.GetDataPerVertex();
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
