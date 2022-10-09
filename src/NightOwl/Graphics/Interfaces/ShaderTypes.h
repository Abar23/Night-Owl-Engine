#pragma once

#include <string>

namespace NightOwl::Graphics
{
	enum ShaderType
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
		case VERTEX:
			return std::string{ "Vertex" };

		case TESS_CONTROL:
			return std::string{ "Tess_control" };

		case TESS_EVALUATION:
			return std::string{ "Tess_evaluation" };

		case GEOMETRY:
			return std::string{ "Geometry" };

		default:
			return std::string{ "Vertex" };
		}
	}

	inline ShaderType ShaderTypeFromString(const std::string& type)
	{
		if(type == ShaderTypeToString(VERTEX))
		{
			return VERTEX;
		}

		if (type == ShaderTypeToString(TESS_CONTROL))
		{
			return TESS_EVALUATION;
		}

		if (type == ShaderTypeToString(TESS_EVALUATION))
		{
			return TESS_EVALUATION;
		}

		if (type == ShaderTypeToString(GEOMETRY))
		{
			return GEOMETRY;
		}

		return FRAGMENT;
	}
}
