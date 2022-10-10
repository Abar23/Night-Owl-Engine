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

		const std::vector<VertexBufferData>& GetBufferDataDefinitions() const;

		void SetBufferDataDefinitions(std::vector<VertexBufferData>& bufferDataDefs);

	private:
		std::vector<VertexBufferData> bufferDataDefinitions;
	};
}
