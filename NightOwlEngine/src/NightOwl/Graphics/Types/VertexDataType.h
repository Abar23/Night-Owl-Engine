#pragma once

namespace NightOwl
{
	enum class VertexDataType
	{
		Position,
		Normal,
		Tangent,
		Bitangent,
		Color,
		Uv0,
		BoneWeights,
		None
	};

	unsigned int VertexDataTypeToNumberOfComponents(VertexDataType type);

	unsigned int VertexDataTypeToDataTypeSize(VertexDataType type);

	int VertexDataTypeToOpenGlComponentType(VertexDataType type);
}
