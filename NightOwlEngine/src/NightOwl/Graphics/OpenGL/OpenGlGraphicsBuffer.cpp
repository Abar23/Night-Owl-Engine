#include "NightOwlPch.h"

#include "OpenGlGraphicsBuffer.h"

#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl
{
	OpenGlGraphicsBuffer::OpenGlGraphicsBuffer(BufferType bufferType):
		IGraphicsBuffer(bufferType)
	{
		openGlBufferType = BufferTypeToOpenGlBufferType(bufferType);
		GL_CALL(glCreateBuffers, 1, &bufferHandle);
	}

	OpenGlGraphicsBuffer::~OpenGlGraphicsBuffer()
	{
		if (wasBufferAllocated)
		{
			ReleaseBuffer();
		}
	}

	void OpenGlGraphicsBuffer::Bind()
	{
		GL_CALL(glBindBuffer, openGlBufferType, bufferHandle);
	}

	void OpenGlGraphicsBuffer::Unbind()
	{
		GL_CALL(glBindBuffer, openGlBufferType, 0);
	}

	void OpenGlGraphicsBuffer::SetData(const void* data)
	{
		ENGINE_ASSERT(wasBufferAllocated, "Trying to write to a buffer that has not been allocated.");

		const unsigned int dataSize = elementCount * stride;
		GL_CALL(glNamedBufferSubData, bufferHandle, 0, dataSize, data);
	}

	void OpenGlGraphicsBuffer::SetSubData(const void* data, unsigned int offsetIntoBuffer, unsigned int subElementCount, unsigned int subElementSize)
	{
		ENGINE_ASSERT(wasBufferAllocated, "Trying to write to a buffer that has not been allocated.");
		ENGINE_ASSERT(subElementCount <= elementCount, "Trying to overwrite more sub elements than elements in the graphics buffer.");

		void* mappedBufferPointer = GL_CALL(glMapNamedBuffer, bufferHandle, GL_WRITE_ONLY);

		ENGINE_ASSERT(mappedBufferPointer != nullptr, "OpenGL mapped buffer pointer is invalid when overwriting buffer sub data.");

		const char* byteDataPointerBegin = static_cast<const char*>(data);
		const char* byteDataPointerEnd = byteDataPointerBegin + (subElementCount * subElementSize);

		char* mappedBufferBytePointerBegin = static_cast<char*>(mappedBufferPointer) + offsetIntoBuffer;

		while(byteDataPointerBegin < byteDataPointerEnd)
		{
			std::memcpy(mappedBufferBytePointerBegin, byteDataPointerBegin, subElementSize);
			mappedBufferBytePointerBegin += stride;
			byteDataPointerBegin += subElementSize;
		}

		GL_CALL(glUnmapNamedBuffer, bufferHandle);
	}

	void OpenGlGraphicsBuffer::ReleaseBuffer()
	{
		GL_CALL(glDeleteBuffers, 1, &bufferHandle);
	}

	void OpenGlGraphicsBuffer::CreateBuffer()
	{
		const unsigned int dataSize = elementCount * stride;
		GL_CALL(glNamedBufferData, bufferHandle, dataSize, nullptr, GL_DYNAMIC_DRAW);
	}
}

