#pragma once

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <shaderc/shaderc.h>

namespace NightOwl
{
	enum class ShaderType
	{
		Vertex,
		TessControl,
		TessEvaluation,
		Geometry,
		Fragment,
		None
	};

	static std::unordered_map<std::string, ShaderType> shaderExtensionToShaderTypeMap = {
		{ "vert", ShaderType::Vertex },
		{ "tesc", ShaderType::TessControl },
		{ "tese", ShaderType::TessEvaluation },
		{ "geom", ShaderType::Geometry },
		{ "frag", ShaderType::Fragment }
	};


	inline std::string ShaderTypeToString(const ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return std::string{ "Vertex" };

		case ShaderType::TessControl:
			return std::string{ "Tess_control" };

		case ShaderType::TessEvaluation:
			return std::string{ "Tess_evaluation" };

		case ShaderType::Geometry:
			return std::string{ "Geometry" };

		case ShaderType::Fragment:
			return std::string{ "Fragment" };

		default:
			return std::string{ "None" };
		}
	}

	inline ShaderType ShaderTypeFromString(const std::string& type)
	{
		if(type == ShaderTypeToString(ShaderType::Vertex))
		{
			return ShaderType::Vertex;
		}

		if (type == ShaderTypeToString(ShaderType::TessControl))
		{
			return ShaderType::TessEvaluation;
		}

		if (type == ShaderTypeToString(ShaderType::TessEvaluation))
		{
			return ShaderType::TessEvaluation;
		}

		if (type == ShaderTypeToString(ShaderType::Geometry))
		{
			return ShaderType::Geometry;
		}

		if (type == ShaderTypeToString(ShaderType::Fragment))
		{
			return ShaderType::Fragment;
		}

		return ShaderType::None;
	}

	inline ShaderType ExtensionToShaderType(const std::string& extension)
	{
		return shaderExtensionToShaderTypeMap.at(extension);
	}

	inline unsigned int ShaderTypeToOpenGlShaderType(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;

		case ShaderType::TessControl:
			return GL_TESS_CONTROL_SHADER;

		case ShaderType::TessEvaluation:
			return GL_TESS_EVALUATION_SHADER;

		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;

		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;

		default:
			return GL_VERTEX_SHADER;
		}
	}

	inline shaderc_shader_kind ShaderTypeToShaderCType(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return shaderc_vertex_shader;

		case ShaderType::TessControl:
			return shaderc_glsl_tess_control_shader;

		case ShaderType::TessEvaluation:
			return shaderc_glsl_tess_evaluation_shader;

		case ShaderType::Geometry:
			return shaderc_geometry_shader;

		case ShaderType::Fragment:
			return shaderc_fragment_shader;

		default:
			return shaderc_vertex_shader;
		}
	}
}
