#pragma once

#include <glad/glad.h>
#include "NightOwl/Graphics/Types/ShaderTypes.h"

namespace NightOwl::Graphics
{
	inline GLenum shaderTypeToGlShaderType(ShaderType type)
	{
		switch (type)
		{
		case VERTEX:
			return GL_VERTEX_SHADER;

		case TESS_CONTROL:
			return GL_TESS_CONTROL_SHADER;

		case TESS_EVALUATION:
			return GL_TESS_EVALUATION_SHADER;

		case GEOMETRY:
			return GL_GEOMETRY_SHADER;

		default:
			return GL_FRAGMENT_SHADER;
		}
	}
}
