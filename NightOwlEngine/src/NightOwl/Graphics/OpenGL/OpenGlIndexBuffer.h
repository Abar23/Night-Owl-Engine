#pragma once

#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"

namespace NightOwl
{
	class OpenGlIndexBuffer : public IIndexBuffer
	{
	public:
		OpenGlIndexBuffer();

		OpenGlIndexBuffer(const void* indexData, unsigned int indexDataSize);

		~OpenGlIndexBuffer() override;

		void Bind() override;

		void Unbind() override;
		
		void SetSize(unsigned indexDataSize) override;

		void SetData(const void* indexData) override;

		unsigned GetIndexBufferId() override;

		unsigned GetIndexBufferDataSize() override;

	private:
		unsigned int indexBufferId;

		unsigned int indexBufferDataSize;
	};
}

