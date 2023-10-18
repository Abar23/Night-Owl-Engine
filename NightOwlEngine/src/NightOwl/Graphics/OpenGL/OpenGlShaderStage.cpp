#include "NightOwlPch.h"

#include "OpenGlShaderStage.h"

#include <shaderc/shaderc.hpp>

#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Structures/ShaderIncluder.h"

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
		shaderId = GL_CALL(glCreateShader, ShaderTypeToOpenGlShaderType(shaderType));

		shaderc::Compiler glslCompiler;
		shaderc::CompileOptions glslOptions;

		glslOptions.SetAutoMapLocations(true);
		glslOptions.SetSourceLanguage(shaderc_source_language_glsl);
		glslOptions.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		glslOptions.SetIncluder(std::make_unique<ShaderIncluder>());

		shaderc::PreprocessedSourceCompilationResult preprocessResult = glslCompiler.PreprocessGlsl(shaderSource, ShaderTypeToShaderCType(shaderType), "test", glslOptions);

		if (preprocessResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			ENGINE_LOG_INFO(preprocessResult.GetErrorMessage());
		}

		std::string glslPreprocessShader(preprocessResult.begin(), preprocessResult.end());

		shaderc::SpvCompilationResult compilationResult = glslCompiler.CompileGlslToSpv(glslPreprocessShader, ShaderTypeToShaderCType(shaderType), "test", glslOptions);
		
		if (compilationResult.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			ENGINE_LOG_INFO(compilationResult.GetErrorMessage());
		}

		std::vector<unsigned int> glslShaderData(compilationResult.begin(), compilationResult.end());

		GL_CALL(glShaderBinary, 1, &shaderId, GL_SHADER_BINARY_FORMAT_SPIR_V, glslShaderData.data(), glslShaderData.size() * sizeof(unsigned int));
		GL_CALL(glSpecializeShader, shaderId, "main", 0, nullptr, nullptr);
	}

	void OpenGlShaderStage::DeleteShaderStage() const
	{
		GL_CALL(glDeleteShader, shaderId);
	}
}
