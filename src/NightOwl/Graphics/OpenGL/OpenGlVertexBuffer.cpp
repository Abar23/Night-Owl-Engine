#include "OpenGlVertexBuffer.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl::Graphics
{
	OpenGlVertexBuffer::OpenGlVertexBuffer(const void* vertexData, unsigned int vertexDataSize)
	{
		this->vertexDataSize = vertexDataSize;
		GL_CALL(glCreateBuffers, 1, &vertexBufferId);
		GL_CALL(glNamedBufferStorage, vertexBufferId, vertexDataSize, vertexData, GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
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

	void OpenGlVertexBuffer::SetData(const void* vertexData, unsigned vertexDataSize)
	{
		this->vertexDataSize = vertexDataSize;
		void* vertexBufferPointer = GL_CALL(glMapNamedBufferRange, vertexBufferId, 0, vertexDataSize, GL_MAP_WRITE_BIT);
		std::memcpy(vertexBufferPointer, vertexData, vertexDataSize);
		GL_CALL(glUnmapNamedBuffer, vertexBufferId);
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
		return vertexDataSize;
	}
}
