#include "OpenGlIndexBuffer.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl::Graphics
{
	OpenGlIndexBuffer::OpenGlIndexBuffer()
		: indexBufferDataSize(0)
	{
		GL_CALL(glCreateBuffers, 1, &indexBufferId);
	}

	OpenGlIndexBuffer::OpenGlIndexBuffer(const void* indexData, unsigned int indexDataSize)
		: indexBufferDataSize(indexDataSize)
	{
		GL_CALL(glCreateBuffers, 1, &indexBufferId);
		GL_CALL(glNamedBufferData, indexBufferId, indexDataSize * sizeof(unsigned int), indexData, GL_DYNAMIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &indexBufferId);
	}

	void OpenGlIndexBuffer::Bind()
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	}

	void OpenGlIndexBuffer::Unbind()
	{
		GL_CALL(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGlIndexBuffer::SetSize(unsigned indexDataSize)
	{
		indexBufferDataSize = indexDataSize;
		GL_CALL(glNamedBufferData, indexBufferId, indexDataSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGlIndexBuffer::SetData(const void* indexData)
	{
		GL_CALL(glNamedBufferSubData, indexBufferId, 0, indexBufferDataSize, indexData);
	}

	unsigned OpenGlIndexBuffer::GetIndexBufferId()
	{
		return indexBufferId;
	}

	unsigned OpenGlIndexBuffer::GetIndexBufferDataSize()
	{
		return indexBufferDataSize;
	}
}
