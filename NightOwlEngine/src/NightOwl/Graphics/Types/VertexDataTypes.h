#pragma once

namespace NightOwl
{
	enum class VertexDataType
	{
		None,
		VectorFloat2,
		VectorInt2,
		VectorInt4,
		VectorFloat3,
		VectorFloat4
	};

	inline unsigned int VertexDataTypeToNumberOfComponents(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::VectorInt2:
		case VertexDataType::VectorFloat2:
			return 2;

		case VertexDataType::VectorFloat3:
			return 3;

		case VertexDataType::VectorInt4:
		case VertexDataType::VectorFloat4:
			return 4;

		default:
			return 0;
		}
	}

	inline unsigned int VertexDataTypeToDataTypeSize(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::VectorInt2:
			return 2 * sizeof(int);

		case VertexDataType::VectorInt4:
			return 4 * sizeof(int);

		case VertexDataType::VectorFloat2:
			return 2 * sizeof(float);

		case VertexDataType::VectorFloat3:
			return 3 * sizeof(float);

		case VertexDataType::VectorFloat4:
			return 4 * sizeof(float);

		default:
			return 0;
		}
	}
}
