#include "VertexBufferLayout.h"

namespace NightOwl::Graphics
{
	VertexBufferLayout::VertexBufferLayout()
		:	dataPerVertex(0)
	{
	}

	VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& bufferDataDefs)
	{
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

	void VertexBufferLayout::AddVertexBufferDataDefinition(const VertexBufferData& vertexBufferDataDef)
	{
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
}

