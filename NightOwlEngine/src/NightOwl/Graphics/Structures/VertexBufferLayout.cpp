#include <NightOwlPch.h>

#include "VertexBufferLayout.h"

namespace NightOwl
{
	VertexBufferLayout::VertexBufferLayout()
		:	dataPerVertex(0)
	{
	}

	VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& bufferDataDefs)
	{
		dataPerVertex = 0;
		bufferDataDefinitions = bufferDataDefs.bufferDataDefinitions;
		for(const VertexBufferData& data : bufferDataDefinitions)
		{
			dataPerVertex += data.GetSizeofData();
		}
		return *this;
	}

	VertexBufferLayout::VertexBufferLayout(std::vector<VertexBufferData>& bufferDataDefs)
		:	bufferDataDefinitions(bufferDataDefs),
			dataPerVertex(0)
	{
		for (const VertexBufferData& data : bufferDataDefinitions)
		{
			dataPerVertex += data.GetSizeofData();
		}
	}

	void VertexBufferLayout::AddVertexBufferDataDefinition(VertexBufferData& vertexBufferDataDef)
	{
		vertexBufferDataDef.SetOffset(dataPerVertex);
		bufferDataDefinitions.push_back(vertexBufferDataDef);
		dataPerVertex += vertexBufferDataDef.GetSizeofData();
	}

	const std::vector<VertexBufferData>& VertexBufferLayout::GetBufferDataDefinitions() const
	{
		return bufferDataDefinitions;
	}

	void VertexBufferLayout::SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs)
	{
		bufferDataDefinitions = bufferDataDefs;
	}

	int VertexBufferLayout::GetDataPerVertex() const
	{
		return dataPerVertex;
	}

	int VertexBufferLayout::GetIndexOfVertexAttribute(VertexDataType vertexDataType) const
	{
		int indexOfShaderAttribute = 0;
		for (const auto& bufferDataDefinition : bufferDataDefinitions)
		{
			if (bufferDataDefinition.GetVertexDataType() == vertexDataType)
			{
				return indexOfShaderAttribute;
			}
			indexOfShaderAttribute++;
		}

		return -1;
	}

	const VertexBufferData* VertexBufferLayout::GetVertexBufferDataForVertexDataType(VertexDataType vertexDataType) const
	{
		for (const auto& bufferDataDefinition : bufferDataDefinitions)
		{
			if (bufferDataDefinition.GetVertexDataType() == vertexDataType)
			{
				return &bufferDataDefinition;
			}
		}

		return nullptr;
	}
}

