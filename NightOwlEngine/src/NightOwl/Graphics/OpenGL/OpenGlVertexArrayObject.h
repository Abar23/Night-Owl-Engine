#pragma once

#include "NightOwl/Graphics/Interfaces/IVertexArrayObject.h"

namespace NightOwl
{
	class OpenGlVertexArrayObject : public IVertexArrayObject
	{
	public:
		OpenGlVertexArrayObject();

		~OpenGlVertexArrayObject() override;

		void Bind() override;

		void Unbind() override;

		void SetVertexBuffer(const std::shared_ptr<IGraphicsBuffer>& graphicsBuffer) override;

		std::shared_ptr<IGraphicsBuffer>& GetVertexBuffer() override;

		void SetIndexBuffer(const std::shared_ptr<IGraphicsBuffer>& graphicsBuffer) override;

		std::shared_ptr<IGraphicsBuffer>& GetIndexBuffer() override;

		unsigned GetVertexArrayObjectId() override;

		void SetupVertexBufferAttributes(const VertexBufferLayout& vertexBufferLayout) override;

	private:
		unsigned int vertexArrayObjectId;

		std::shared_ptr<IGraphicsBuffer> vertexBuffer;

		std::shared_ptr<IGraphicsBuffer> indexBuffer;
	};
}
