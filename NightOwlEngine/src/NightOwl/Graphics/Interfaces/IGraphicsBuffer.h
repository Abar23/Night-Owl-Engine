#pragma once

#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Graphics/Types/BufferType.h"
#include <string>

namespace NightOwl
{
	class VertexBufferData;

	class IGraphicsBuffer
	{
	public:
		IGraphicsBuffer(BufferType bufferType)
			: bufferHandle(-1), elementCount(0), stride(0), bufferType(bufferType), wasBufferAllocated(0)
		{}

		virtual ~IGraphicsBuffer() = default;

		virtual void Bind() = 0;

		virtual void Bind(unsigned int bindingPoint) = 0;

		virtual void Unbind() = 0;

		void SetSize(unsigned int elementCount, unsigned int stride)
		{
			//ENGINE_ASSERT(elementCount > 0 && stride > 0, "elementCount and stride must be greater than 0 when setting size of graphics buffer.");

			this->elementCount = elementCount;
			this->stride = stride;

			CreateBuffer();

			wasBufferAllocated = true;
		}

		virtual void SetData(const void* data) = 0;

		virtual void SetSubData(const void* data, unsigned int offsetIntoBuffer, unsigned int subElementCount, unsigned int subElementSize) = 0;

		unsigned int GetBufferHandle() const { return bufferHandle; }

		unsigned int GetElementCount() const { return elementCount; }

		unsigned int GetStride() const { return stride; }

		const std::string& GetName() const { return name; }

		void SetName(const std::string& name) { this->name = name; }

		BufferType GetBufferType() const { return bufferType; }

	protected:
		unsigned int bufferHandle;

		unsigned int elementCount;

		unsigned int stride;

		bool wasBufferAllocated;

		BufferType bufferType;

		std::string name; // Debug usage only

		virtual void ReleaseBuffer() = 0;

		virtual void CreateBuffer() = 0;
	};
}
