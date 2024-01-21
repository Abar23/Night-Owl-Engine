#pragma once

#include <string>
#include <glad/glad.h>

namespace NightOwl
{
	enum class BufferType
	{
		Vertex,
		Index,
		Uniform,
		Storage,
		None
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

	inline std::string BufferTypeToString(BufferType type)
	{
		switch (type)
		{
		case BufferType::Vertex:
			return std::string{ "Vertex" };

		case BufferType::Index:
			return std::string{ "Index" };

		case BufferType::Uniform:
			return std::string{ "Uniform" };

		case BufferType::Storage:
			return std::string{ "Storage" };
		}

		return std::string{ "None" };
	}
}
