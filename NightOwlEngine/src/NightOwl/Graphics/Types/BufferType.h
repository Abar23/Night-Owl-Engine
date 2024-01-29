#pragma once

#include <string>

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

	int BufferTypeToOpenGlBufferType(BufferType bufferType);

	std::string BufferTypeToString(BufferType type);
}
