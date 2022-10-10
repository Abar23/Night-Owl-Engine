#include "OpenGlIndexBuffer.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl::Graphics
{
	OpenGlIndexBuffer::OpenGlIndexBuffer(const void* indexData, unsigned int indexDataSize)
		: indexDataSize(indexDataSize)
	{
		GL_CALL(glCreateBuffers, 1, &indexBufferId);
		GL_CALL(glNamedBufferStorage, indexBufferId, indexDataSize * sizeof(unsigned int), indexData, GL_MAP_READ_BIT);
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

	unsigned OpenGlIndexBuffer::GetIndexBufferId()
	{
		return indexBufferId;
	}

	unsigned OpenGlIndexBuffer::GetIndexBufferDataSize()
	{
		return indexDataSize;
	}
}
