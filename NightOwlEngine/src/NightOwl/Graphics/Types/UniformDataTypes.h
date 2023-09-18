#pragma once

#include <string>

#include "ShaderTypes.h"
#include "glad/glad.h"

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

	inline UniformDataTypes OpenGlUniformToUniformDataType(const unsigned int type)
	{
        switch (type)
		{
        case GL_FLOAT:
			return UniformDataTypes::Float;

		case GL_FLOAT_VEC4:
			return UniformDataTypes::Vec4F;

		case GL_INT:
        case GL_BOOL:
			return UniformDataTypes::Int;

		case GL_FLOAT_MAT4:
			return UniformDataTypes::Mat4F;

		case GL_SAMPLER_2D:
			return UniformDataTypes::Texture;

		default:
			return UniformDataTypes::None;
        }
	}

	inline std::string UniformDataTypeToString(const UniformDataTypes type)
	{
		switch (type)
		{
		case UniformDataTypes::Float:
			return std::string{ "Float" };

		case UniformDataTypes::Vec4F:
			return std::string{ "Vec4F" };

		case UniformDataTypes::Int:
			return std::string{ "Int" };

		case UniformDataTypes::Mat4F:
			return std::string{ "Mat4F" };

		case UniformDataTypes::Texture:
			return std::string{ "Texture" };

		case UniformDataTypes::Buffer:
			return std::string{ "Buffer" };

		default:
			return std::string{ "None" };
		}
	}
}
