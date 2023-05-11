#pragma once

#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"

namespace NightOwl::Graphics
{
	class OpenGlVertexArrayObject : public IVertexArrayObject
	{
	public:
		OpenGlVertexArrayObject();

		~OpenGlVertexArrayObject() override;

		void Bind() override;

		void Unbind() override;

		void SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;

		std::shared_ptr<IVertexBuffer>& GetVertexBuffer() override;

		void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;

		std::shared_ptr<IIndexBuffer>& GetIndexBuffer() override;

		unsigned GetVertexArrayObjectId() override;

		void SetupVertexBufferAttributes();

	private:
		unsigned int vertexArrayObjectId;

		std::shared_ptr<IVertexBuffer> vertexBuffer;

		std::shared_ptr<IIndexBuffer> indexBuffer;
	};
}
