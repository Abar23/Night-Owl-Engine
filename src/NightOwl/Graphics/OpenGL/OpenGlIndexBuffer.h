#pragma once

#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"

namespace NightOwl::Graphics
{
	class OpenGlIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGlIndexBuffer() = default;

		OpenGlIndexBuffer(const void* indexData, unsigned int indexDataSize);

		~OpenGlIndexBuffer() override;

		void Bind() override;

		void Unbind() override;

		unsigned GetIndexBufferId() override;

		unsigned GetIndexBufferDataSize() override;

	private:
		unsigned int indexBufferId;

		unsigned int indexDataSize;
	};
}

