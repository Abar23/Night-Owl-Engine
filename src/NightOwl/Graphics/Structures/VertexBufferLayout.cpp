#include "VertexBufferLayout.h"

namespace NightOwl::Graphics
{
	VertexBufferLayout& VertexBufferLayout::operator=(const VertexBufferLayout& bufferDataDefs)
	{
		bufferDataDefinitions = bufferDataDefs.bufferDataDefinitions;
		return *this;
	}

	VertexBufferLayout::VertexBufferLayout(std::vector<VertexBufferData>& bufferDataDefs)
		: bufferDataDefinitions(bufferDataDefs)
	{
	}

	const std::vector<VertexBufferData>& VertexBufferLayout::GetBufferDataDefinitions() const
	{
		return bufferDataDefinitions;
	}

	void VertexBufferLayout::SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs)
	{
		bufferDataDefinitions = bufferDataDefs;
	}
}

