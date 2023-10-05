#include <NightOwlPch.h>

#include "OpenGlVertexArrayObject.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Interfaces/IIndexBuffer.h"
#include "NightOwl/Graphics/Interfaces/IVertexBuffer.h"
#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include "NightOwl/Graphics/Structures/VertexBufferLayout.h"

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

	void OpenGlVertexArrayObject::SetVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer)
	{
		this->vertexBuffer = vertexBuffer;
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
		unsigned int accumulativeOffset = 0;
		for(const VertexBufferData& data : layout.GetBufferDataDefinitions())
		{
			unsigned int openGlDataType;
			switch (data.GetVertexDataType())
			{
			case VertexDataType::VectorFloat2:
			case VertexDataType::VectorFloat3:
			case VertexDataType::VectorFloat4:
				openGlDataType = GL_FLOAT;
				break;

			case VertexDataType::VectorInt2:
			case VertexDataType::VectorInt4:
				openGlDataType = GL_INT;
				break;

			default:
				openGlDataType = 0;
				break;
			}

			if (openGlDataType > 0)
			{
				GL_CALL(glVertexArrayAttribBinding, vertexArrayObjectId, data.GetAttributeLocation(), 0);

				if (openGlDataType == GL_FLOAT)
				{
					GL_CALL(glVertexArrayAttribFormat, vertexArrayObjectId, data.GetAttributeLocation(), data.GetNumberOfComponents(), openGlDataType, data.GetNormalize(), accumulativeOffset);
				}
				else
				{
					GL_CALL(glVertexArrayAttribIFormat, vertexArrayObjectId, data.GetAttributeLocation(), data.GetNumberOfComponents(), openGlDataType, accumulativeOffset);
				}

				GL_CALL(glEnableVertexArrayAttrib, vertexArrayObjectId, data.GetAttributeLocation());
				accumulativeOffset += data.GetSizeofData();
			}
		}
		GL_CALL(glVertexArrayVertexBuffer, vertexArrayObjectId, 0, vertexBuffer->GetVertexBufferId(), 0, layout.GetDataPerVertex());
	}
}
