#pragma once

namespace NightOwl::Graphics
{
	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetSize(unsigned int indexDataSize) = 0;

		virtual void SetData(const void* indexData) = 0;

		virtual unsigned int GetIndexBufferId() = 0;

		virtual unsigned int GetIndexBufferDataSize() = 0;
	};
}
