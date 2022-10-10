#pragma once

namespace NightOwl::Graphics
{
	enum VertexDataType
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
		case VectorInt2:
		case VectorFloat2:
			return 2;

		case VectorFloat3:
			return 3;

		default:
			return 0;
		}
	}

	inline unsigned int VertexDataTypeToDataTypeSize(VertexDataType type)
	{
		switch (type)
		{
		case VectorInt2:
			return 2 * sizeof(int);

		case VectorFloat2:
			return 2 * sizeof(float);

		case VectorFloat3:
			return 3 * sizeof(float);

		default:
			return 0;
		}
	}
}
