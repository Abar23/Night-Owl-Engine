#pragma once

namespace NightOwl
{
	enum class VertexDataType
	{
		None,
		VectorFloat2,
		VectorInt2,
		VectorFloat3
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

		case VertexDataType::VectorFloat2:
			return 2 * sizeof(float);

		case VertexDataType::VectorFloat3:
			return 3 * sizeof(float);

		default:
			return 0;
		}
	}
}
