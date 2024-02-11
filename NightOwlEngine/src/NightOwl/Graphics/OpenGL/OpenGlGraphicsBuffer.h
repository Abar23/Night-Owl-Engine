#pragma once
#include "NightOwl/Graphics/Interfaces/IGraphicsBuffer.h"

namespace NightOwl
{
	class OpenGlGraphicsBuffer : public IGraphicsBuffer
	{
	public:
		OpenGlGraphicsBuffer(BufferType bufferType);

		~OpenGlGraphicsBuffer() override;

		void Bind() override;

		void Bind(unsigned int bindingPoint) override;

		void Unbind() override;

		void SetData(const void* data) override;

		void SetSubData(const void* data, unsigned int offsetIntoBuffer, unsigned int subElementCount, unsigned int subElementSize) override;

	protected:
		void ReleaseBuffer() override;

		void CreateBuffer() override;

	private:
		unsigned int openGlBufferType;

		unsigned int bufferBindingPoint;
	};
}
