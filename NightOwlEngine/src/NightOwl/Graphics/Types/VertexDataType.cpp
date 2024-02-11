#include "NightOwlPch.h"

#include "VertexDataType.h"
#include "NightOwl/Animation/3D/Structures/BoneWeight.h"
#include "NightOwl/Math/Vec2.h"
#include "NightOwl/Math/Vec3.h"
#include "NightOwl/Math/Vec4.h"
#include <glad/glad.h>

namespace NightOwl
{
	unsigned int VertexDataTypeToNumberOfComponents(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::Uv0:
			return 2;

		case VertexDataType::Position:
		case VertexDataType::Normal:
		case VertexDataType::Color:
			return 3;

		case VertexDataType::Tangent:
		case VertexDataType::Bitangent:
			return 4;

		case VertexDataType::BoneWeights:
			return 8;

		default:
			return 0;
		}
	}

	unsigned int VertexDataTypeToDataTypeSize(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::Uv0:
			return sizeof(Vec2F);

		case VertexDataType::Position:
		case VertexDataType::Normal:
		case VertexDataType::Color:
		case VertexDataType::Tangent:
		case VertexDataType::Bitangent:
			return sizeof(Vec3F);

		case VertexDataType::BoneWeights:
			return sizeof(BoneWeight);

		default:
			return 0;
		}
	}

	int VertexDataTypeToOpenGlComponentType(VertexDataType type)
	{
		switch (type)
		{
		case VertexDataType::Uv0:
		case VertexDataType::Position:
		case VertexDataType::Normal:
		case VertexDataType::Color:
		case VertexDataType::Tangent:
		case VertexDataType::Bitangent:
			return GL_FLOAT;

		default:
			return 0;
		}
	}
}