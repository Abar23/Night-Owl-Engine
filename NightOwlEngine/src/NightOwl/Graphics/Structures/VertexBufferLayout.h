#pragma once

#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include <vector>

namespace NightOwl
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();

		VertexBufferLayout& operator=(const VertexBufferLayout& bufferDataDefs);

		VertexBufferLayout(std::vector<VertexBufferData>& bufferDataDefs);

		void AddVertexBufferDataDefinition(VertexBufferData& vertexBufferDataDef);

		const std::vector<VertexBufferData>& GetBufferDataDefinitions() const;

		void SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs);

		int GetDataPerVertex() const;

		int GetIndexOfShaderAttribute(const std::string& name) const;

	private:
		std::vector<VertexBufferData> bufferDataDefinitions;

		int dataPerVertex;
	};
}