#pragma once

#include "NightOwl/Graphics/Structures/VertexBufferData.h"
#include <vector>

namespace NightOwl::Graphics
{
	class VertexBufferLayout
	{
	public:
		VertexBufferLayout() = default;

		VertexBufferLayout& operator=(const VertexBufferLayout& bufferDataDefs);

		VertexBufferLayout(std::vector<VertexBufferData>& bufferDataDefs);

		void AddVertexBufferDataDefinition(const VertexBufferData& vertexBufferDataDef);

		const std::vector<VertexBufferData>& GetBufferDataDefinitions() const;

		void SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs);

		int GetDataPerTriangle() const;

	private:
		std::vector<VertexBufferData> bufferDataDefinitions;

		int dataPerTriangle;
	};
}
