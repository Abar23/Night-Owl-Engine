#include "VertexBufferLayout.h"

namespace NightOwl::Graphics
{
	VertexBufferLayout::VertexBufferLayout()
		: dataPerTriangle(0)
	{
	}

	VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& bufferDataDefs)
	{
		bufferDataDefinitions = bufferDataDefs.bufferDataDefinitions;
		for(const VertexBufferData& data : bufferDataDefinitions)
		{
			dataPerTriangle += data.GetSizeofData();
		}
		return *this;
	}

	VertexBufferLayout::VertexBufferLayout(std::vector<VertexBufferData>& bufferDataDefs)
		: bufferDataDefinitions(bufferDataDefs), dataPerTriangle(0)
	{
		for (const VertexBufferData& data : bufferDataDefinitions)
		{
			dataPerTriangle += data.GetSizeofData();
		}
	}

	void VertexBufferLayout::AddVertexBufferDataDefinition(const VertexBufferData& vertexBufferDataDef)
	{
		bufferDataDefinitions.push_back(vertexBufferDataDef);
		dataPerTriangle += vertexBufferDataDef.GetSizeofData();
	}

	const std::vector<VertexBufferData>& VertexBufferLayout::GetBufferDataDefinitions() const
	{
		return bufferDataDefinitions;
	}

	void VertexBufferLayout::SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs)
	{
		bufferDataDefinitions = bufferDataDefs;
	}

	int VertexBufferLayout::GetDataPerTriangle() const
	{
		return dataPerTriangle;
	}
}

