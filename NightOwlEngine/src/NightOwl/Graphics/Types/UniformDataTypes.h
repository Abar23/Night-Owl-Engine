#pragma once

#include <string>

namespace NightOwl
{
	enum class UniformDataTypes
	{
		Float,
        Vec4F,
        Mat4F,
        Int,
        Texture,
		Buffer,
		NumberOfTypes,
		None
	};

	UniformDataTypes OpenGlUniformToUniformDataType(const unsigned int type);

	std::string UniformDataTypeToString(const UniformDataTypes type);
}
