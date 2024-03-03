#include "NightOwlPch.h"

#include "ShaderType.h"
#include <glad/glad.h>

namespace NightOwl
{
	std::string ShaderTypeToString(const ShaderType type)
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

		case ShaderType::Compute:
			return std::string{ "Compute" };

		default:
			return std::string{ "None" };
		}
	}

	ShaderType ShaderTypeFromString(const std::string& type)
	{
		if (type == ShaderTypeToString(ShaderType::Vertex))
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

	ShaderType ExtensionToShaderType(const std::string& extension)
	{
		return shaderExtensionToShaderTypeMap.at(extension);
	}
	
	unsigned int ShaderTypeToOpenGlShaderType(ShaderType type)
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

		case ShaderType::Compute:
			return GL_COMPUTE_SHADER;

		default:
			return GL_VERTEX_SHADER;
		}
	}

	shaderc_shader_kind ShaderTypeToShaderCType(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::Vertex:
			return shaderc_vertex_shader;

		case ShaderType::TessControl:
			return shaderc_tess_control_shader;

		case ShaderType::TessEvaluation:
			return shaderc_tess_evaluation_shader;

		case ShaderType::Geometry:
			return shaderc_geometry_shader;

		case ShaderType::Fragment:
			return shaderc_fragment_shader;

		case ShaderType::Compute:
			return shaderc_compute_shader;

		default:
			return shaderc_vertex_shader;
		}
	}
}
