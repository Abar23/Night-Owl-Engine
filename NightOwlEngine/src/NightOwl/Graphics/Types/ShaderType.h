#pragma once

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
		Compute,
		None
	};

	static std::unordered_map<std::string, ShaderType> shaderExtensionToShaderTypeMap = {
		{ "vert", ShaderType::Vertex },
		{ "tesc", ShaderType::TessControl },
		{ "tese", ShaderType::TessEvaluation },
		{ "geom", ShaderType::Geometry },
		{ "frag", ShaderType::Fragment },
		{ "comp", ShaderType::Compute}
	};

	std::string ShaderTypeToString(const ShaderType type);

	ShaderType ShaderTypeFromString(const std::string& type);

	ShaderType ExtensionToShaderType(const std::string& extension);

	unsigned int ShaderTypeToOpenGlShaderType(ShaderType type);

	shaderc_shader_kind ShaderTypeToShaderCType(ShaderType type);
}
