#pragma once

#include "NightOwl/Graphics/Types/BufferType.h"
#include <string>

namespace NightOwl
{
	class IGraphicsBuffer
	{
	public:
		virtual ~IGraphicsBuffer() = default;

	private:
		unsigned int bufferHandle;

		unsigned int elementsInBuffer;

		std::string name; // Debug usage only

		BufferType bufferType;
	};
}
