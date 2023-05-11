#include <NightOwlPch.h>

#include "OpenGLShader.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"

namespace NightOwl::Graphics
{
	OpenGlShader::OpenGlShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
		:	programId(0),
			name{ name }
	{
		const unsigned int vertexShaderId = CompileShaderSource(vertexShaderSource, GL_VERTEX_SHADER);
		const unsigned int fragmentShaderId = CompileShaderSource(fragmentShaderSource, GL_FRAGMENT_SHADER);
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

	void OpenGlShader::SetUniformInt(int value, const std::string& uniformName)
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void OpenGlShader::SetUniformFloat(float value, const std::string& uniformName)
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	int OpenGlShader::GetShaderId()
	{
		return programId;
	}

	const std::string& OpenGlShader::GetShaderName()
	{
		return name;
	}

	unsigned int OpenGlShader::CompileShaderSource(const std::string& shaderSource, GLenum shaderType)
	{
		const char* acceptableShaderSource = shaderSource.c_str();
		const unsigned int shaderId = GL_CALL(glCreateShader, shaderType);
		GL_CALL(glShaderSource, shaderId, 1, &acceptableShaderSource, NULL);
		GL_CALL(glCompileShader, shaderId);
		CHECK_SHADER_COMPILER_ERRORS(shaderId);
		return shaderId;
	}

	unsigned int OpenGlShader::GetUniformLocation(const std::string& uniformName) const
	{
		int i = glGetUniformLocation(programId, uniformName.c_str());
		return GL_CALL(glGetUniformLocation, programId, uniformName.c_str());
	}
}
