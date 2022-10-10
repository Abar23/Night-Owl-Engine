#include "OpenGlVertexArrayObject.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"

namespace NightOwl::Graphics
{
	OpenGlVertexArrayObject::OpenGlVertexArrayObject()
	{
		GL_CALL(glCreateVertexArrays, 1, &vertexArrayObjectId);
	}

	OpenGlVertexArrayObject::~OpenGlVertexArrayObject()
	{
		GL_CALL(glDeleteVertexArrays, 1, &vertexArrayObjectId);
	}

	void OpenGlVertexArrayObject::Bind()
	{
		GL_CALL(glBindVertexArray, vertexArrayObjectId);
	}

	void OpenGlVertexArrayObject::Unbind()
	{
		GL_CALL(glBindVertexArray, 0);
	}

	void OpenGlVertexArrayObject::SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer)
	{
		this->vertexBuffer = vertexBuffer;
		SetupVertexBufferAttributes();
	}

	std::shared_ptr<IVertexBuffer>& OpenGlVertexArrayObject::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	void OpenGlVertexArrayObject::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer)
	{
		this->indexBuffer = indexBuffer;
		GL_CALL(glVertexArrayElementBuffer, vertexArrayObjectId, indexBuffer->GetIndexBufferId());
	}

	std::shared_ptr<IIndexBuffer>& OpenGlVertexArrayObject::GetIndexBuffer()
	{
		return indexBuffer;
	}

	unsigned OpenGlVertexArrayObject::GetVertexArrayObjectId()
	{
		return vertexArrayObjectId;
	}

	void OpenGlVertexArrayObject::SetupVertexBufferAttributes()
	{
		const VertexBufferLayout& layout = vertexBuffer->GetVertexBufferLayout();
		unsigned int attributeLocation = 0;
		unsigned int accumulativeOffset = 0;
		for(const VertexBufferData& data : layout.GetBufferDataDefinitions() )
		{
			// Only supporting floats for the moment
			switch (data.GetVertexDataType())
			{
			case VectorFloat2:
			case VectorFloat3:
				GL_CALL(glVertexArrayAttribBinding, vertexArrayObjectId, attributeLocation, 0);
				GL_CALL(glVertexArrayAttribFormat, vertexArrayObjectId, attributeLocation, data.GetNumberOfComponents(), GL_FLOAT, data.GetNormalize(), accumulativeOffset);
				GL_CALL(glEnableVertexArrayAttrib, vertexArrayObjectId, attributeLocation);
				attributeLocation++;
				accumulativeOffset += data.GetSizeofData();
			}
		}
		GL_CALL(glVertexArrayVertexBuffer, vertexArrayObjectId, 0, vertexBuffer->GetVertexBufferId(), 0, layout.GetDataPerTriangle());
	}
}
