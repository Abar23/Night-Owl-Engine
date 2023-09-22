#pragma once

#include <glad/glad.h>
#include "NightOwl/Graphics/Types/ShaderTypes.h"

namespace NightOwl
{
	inline GLenum shaderTypeToGlShaderType(ShaderType type)
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

		default:
			return GL_FRAGMENT_SHADER;
		}
	}
}
