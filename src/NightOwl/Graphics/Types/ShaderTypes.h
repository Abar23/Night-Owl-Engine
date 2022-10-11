#pragma once

#include <string>

namespace NightOwl::Graphics
{
	enum class ShaderType
	{
		VERTEX,
		TESS_CONTROL,
		TESS_EVALUATION,
		GEOMETRY,
		FRAGMENT
	};

	inline std::string ShaderTypeToString(const ShaderType type)
	{
		switch (type)
		{
		case ShaderType::VERTEX:
			return std::string{ "Vertex" };

		case ShaderType::TESS_CONTROL:
			return std::string{ "Tess_control" };

		case ShaderType::TESS_EVALUATION:
			return std::string{ "Tess_evaluation" };

		case ShaderType::GEOMETRY:
			return std::string{ "Geometry" };

		default:
			return std::string{ "Vertex" };
		}
	}

	inline ShaderType ShaderTypeFromString(const std::string& type)
	{
		if(type == ShaderTypeToString(ShaderType::VERTEX))
		{
			return ShaderType::VERTEX;
		}

		if (type == ShaderTypeToString(ShaderType::TESS_CONTROL))
		{
			return ShaderType::TESS_EVALUATION;
		}

		if (type == ShaderTypeToString(ShaderType::TESS_EVALUATION))
		{
			return ShaderType::TESS_EVALUATION;
		}

		if (type == ShaderTypeToString(ShaderType::GEOMETRY))
		{
			return ShaderType::GEOMETRY;
		}

		return ShaderType::FRAGMENT;
	}
}
