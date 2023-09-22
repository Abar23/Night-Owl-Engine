#include "NightOwlPch.h"

#include "OpenGlShaderStage.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"

namespace NightOwl
{
	OpenGlShaderStage::OpenGlShaderStage()
		: shaderId(0),
		  shaderType(ShaderType::None)
	{
	}

	OpenGlShaderStage::OpenGlShaderStage(const std::string& shaderSource, ShaderType shaderType)
		: shaderId(0),
		  shaderType(shaderType)
	{
		CompileShaderSource(shaderSource);
	}

	OpenGlShaderStage::~OpenGlShaderStage()
	{
		DeleteShaderStage();
	}

	unsigned OpenGlShaderStage::GetId()
	{
		return shaderId;
	}

	ShaderType OpenGlShaderStage::GetType()
	{
		return shaderType;
	}

	void OpenGlShaderStage::SetShaderSource(const std::string& shaderSource, ShaderType shaderType)
	{
		if (shaderType != ShaderType::None)
		{
			DeleteShaderStage();
		}
		this->shaderType = shaderType;

		CompileShaderSource(shaderSource);
	}

	void OpenGlShaderStage::CompileShaderSource(const std::string& shaderSource)
	{
		const char* acceptableShaderSource = shaderSource.c_str();
		shaderId = GL_CALL(glCreateShader, ShaderTypeToOpenGlShaderType(shaderType));
		GL_CALL(glShaderSource, shaderId, 1, &acceptableShaderSource, NULL);
		GL_CALL(glCompileShader, shaderId);

		CHECK_SHADER_COMPILER_ERRORS(shaderId);
	}

	void OpenGlShaderStage::DeleteShaderStage() const
	{
		GL_CALL(glDeleteShader, shaderId);
	}
}
