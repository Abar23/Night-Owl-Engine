#include <NightOwlPch.h>

#include "OpenGlVertexArrayObject.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Interfaces/IGraphicsBuffer.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"
#include "NightOwl/Graphics/Types/VertexDataTypes.h"

namespace NightOwl
{
	OpenGlVertexArrayObject::OpenGlVertexArrayObject()
		:	vertexArrayObjectId(0)
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

	void OpenGlVertexArrayObject::SetVertexBuffer(const std::shared_ptr<IGraphicsBuffer>& graphicsBuffer)
	{
		ENGINE_ASSERT(graphicsBuffer->GetBufferType() == BufferType::Vertex, "OpenGL array object expected Vertex buffer type but recieved {0}", BufferTypeToString(graphicsBuffer->GetBufferType()));

		this->vertexBuffer = graphicsBuffer;
	}

	std::shared_ptr<IGraphicsBuffer>& OpenGlVertexArrayObject::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	void OpenGlVertexArrayObject::SetIndexBuffer(const std::shared_ptr<IGraphicsBuffer>& graphicsBuffer)
	{
		ENGINE_ASSERT(graphicsBuffer->GetBufferType() == BufferType::Index, "OpenGL array object expected Index buffer type but recieved {0}", BufferTypeToString(graphicsBuffer->GetBufferType()));

		this->indexBuffer = graphicsBuffer;
		GL_CALL(glVertexArrayElementBuffer, vertexArrayObjectId, graphicsBuffer->GetBufferHandle());
	}

	std::shared_ptr<IGraphicsBuffer>& OpenGlVertexArrayObject::GetIndexBuffer()
	{
		return indexBuffer;
	}

	unsigned OpenGlVertexArrayObject::GetVertexArrayObjectId()
	{
		return vertexArrayObjectId;
	}

	void OpenGlVertexArrayObject::SetupVertexBufferAttributes(const VertexBufferLayout& vertexBufferLayout)
	{
		this->vertexBufferLayout = vertexBufferLayout;

		unsigned int accumulativeOffset = 0;
		for (const VertexBufferData& data : vertexBufferLayout.GetBufferDataDefinitions())
		{
			// Special Handling of BoneWeights
			if (data.GetVertexDataType() == VertexDataType::BoneWeights)
			{
				GL_CALL(glVertexArrayAttribFormat, vertexArrayObjectId, data.GetAttributeLocation(), 4, GL_FLOAT, GL_FALSE, accumulativeOffset);
				GL_CALL(glVertexArrayAttribIFormat, vertexArrayObjectId, data.GetAttributeLocation() + 1, 4, GL_INT, accumulativeOffset);

				accumulativeOffset += data.GetSizeofData();
				continue;
			}

			const unsigned int openGlDataType = VertexDataTypeToOpenGlComponentType(data.GetVertexDataType());

			GL_CALL(glVertexArrayAttribBinding, vertexArrayObjectId, data.GetAttributeLocation(), 0);
			GL_CALL(glVertexArrayAttribFormat, vertexArrayObjectId, data.GetAttributeLocation(), data.GetNumberOfComponents(), openGlDataType, GL_FALSE, accumulativeOffset);
			GL_CALL(glEnableVertexArrayAttrib, vertexArrayObjectId, data.GetAttributeLocation());

			accumulativeOffset += data.GetSizeofData();
		}

		GL_CALL(glVertexArrayVertexBuffer, vertexArrayObjectId, 0, vertexBuffer->GetBufferHandle(), 0, vertexBufferLayout.GetDataPerVertex());
	}
}
