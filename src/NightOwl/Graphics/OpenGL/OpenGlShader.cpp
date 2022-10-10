#include "OpenGLShader.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include <fstream>
#include <iostream>

namespace NightOwl::Graphics
{
	OpenGlShader::OpenGlShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
		: programId(0), name{ name }
	{
		const unsigned int vertexShaderId = CompileShaderSource(vertexShaderPath, GL_VERTEX_SHADER);
		const unsigned int fragmentShaderId = CompileShaderSource(fragmentShaderPath, GL_FRAGMENT_SHADER);
		programId = GL_CALL(glCreateProgram);
		GL_CALL(glAttachShader, programId, vertexShaderId);
		GL_CALL(glAttachShader, programId, fragmentShaderId);
		GL_CALL(glLinkProgram, programId);
		CHECK_PROGRAM_LINKER_ERRORS(programId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	OpenGlShader::~OpenGlShader()
	{
		GL_CALL(glUseProgram, 0);
		GL_CALL(glDeleteProgram, programId);
	}

	void OpenGlShader::Bind()
	{
		GL_CALL(glUseProgram, programId);
	}

	void OpenGlShader::Unbind()
	{
		GL_CALL(glUseProgram, 0);
	}

	void OpenGlShader::SetUniformMat4F(const Math::Mat4F& mat4, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, false, mat4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec4F(const Math::Vec4F& vec4, const std::string& uniformName)
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3F(const Math::Vec3F& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3UI(const Math::Vec3UI& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec2F(const Math::Vec2F& vec2, const std::string& uniformName)
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetValuePointer());
	}

	void OpenGlShader::SetUniformInt(const int id, const std::string& uniformName)
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), id);
	}

	int OpenGlShader::GetShaderId()
	{
		return programId;
	}

	const std::string& OpenGlShader::GetShaderName()
	{
		return name;
	}

	unsigned int OpenGlShader::CompileShaderSource(const std::string& sourcePath, GLenum shaderType)
	{
		const std::string shaderSource = GetShaderSource(sourcePath);
		const char* acceptableShaderSource = shaderSource.c_str();
		const unsigned int shaderId = GL_CALL(glCreateShader, shaderType);
		GL_CALL(glShaderSource, shaderId, 1, &acceptableShaderSource, NULL);
		GL_CALL(glCompileShader, shaderId);
		CHECK_SHADER_COMPILER_ERRORS(shaderId);
		return shaderId;
	}

	std::string OpenGlShader::GetShaderSource(const std::string& sourcePath)
	{
		std::string shaderSource;
		try
		{
			std::ifstream shaderSourceFile;

			shaderSourceFile.exceptions(std::ios::failbit | std::ios::badbit);
			shaderSourceFile.open(sourcePath, std::ios::binary);

			shaderSourceFile.seekg(0, std::ios::end);
			const std::streamsize shaderSourceFileSize = shaderSourceFile.tellg();

			if(shaderSourceFileSize > 0)
			{
				shaderSource.resize(shaderSourceFileSize);
				shaderSourceFile.seekg(0, std::ios::beg);
				shaderSourceFile.read(shaderSource.data(), shaderSourceFileSize);
			}
			else
			{
				ENGINE_LOG_ERROR("Shader source file is empty: {0}", sourcePath);
				std::terminate();
			}
		}
		catch (std::exception& e)
		{
			ENGINE_LOG_ERROR("Failed to open shader source file: {0}\n Exception raised: {1}\n, Error raised: {2}", sourcePath, e.what(), strerror(errno));
			std::terminate();
		}

		return shaderSource;
	}

	unsigned int OpenGlShader::GetUniformLocation(const std::string& uniformName) const
	{
		return GL_CALL(glGetUniformLocation, programId, uniformName.c_str());
	}
}
