#pragma once

#include <glad/glad.h>

namespace NightOwl
{
	enum class BufferType
	{
		Vertex,
		Index,
		Uniform,
		Storage
	};

	inline int BufferTypeToOpenGlBufferType(BufferType bufferType)
	{
		switch (bufferType)
		{
		case BufferType::Vertex:
			return GL_ARRAY_BUFFER;

		case BufferType::Index:
			return GL_ELEMENT_ARRAY_BUFFER;

		case BufferType::Uniform:
			return GL_UNIFORM_BUFFER;

		case BufferType::Storage:
			return GL_SHADER_STORAGE_BUFFER;
		}
	}
}
