#include "NightOwlPch.h"

#include "OpenGlComputeShader.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include <glad/glad.h>

#include "NightOwl/Graphics/Interfaces/IShaderStage.h"

namespace NightOwl
{
	OpenGlComputeShader::OpenGlComputeShader(const std::string& name)
		: name{ name },
		  programId(std::numeric_limits<unsigned int>::max())
	{
		programId = GL_CALL(glCreateProgram);
	}

	OpenGlComputeShader::~OpenGlComputeShader()
	{
		GL_CALL(glUseProgram, 0);
		GL_CALL(glDeleteProgram, programId);
	}

	void OpenGlComputeShader::Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ) const
	{
		GL_CALL(glDispatchCompute, threadGroupsX, threadGroupsY, threadGroupsZ);
	}

	void OpenGlComputeShader::Bind() const
	{
		GL_CALL(glUseProgram, programId);
	}

	void OpenGlComputeShader::Unbind() const
	{
		GL_CALL(glUseProgram, 0);
	}

	void OpenGlComputeShader::AttachAndLinkComputeShaderStage(const std::shared_ptr<IShaderStage>& shaderStage) const
	{
		ENGINE_ASSERT(shaderStage->GetType() == ShaderType::Compute, "Trying to link and attach non-compute shader stage to computer shader program.");

		GL_CALL(glAttachShader, programId, shaderStage->GetId());

		GL_CALL(glLinkProgram, programId);

		CHECK_PROGRAM_LINKER_ERRORS(programId);
	}

	void OpenGlComputeShader::SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) const
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, false, mat4.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformMat4F(const Mat4F& mat4, int uniformId) const
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, mat4.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) const
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec4F(const Vec4F& vec4, int uniformId) const
	{
		GL_CALL(glUniform4fv, uniformId, 1, vec4.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) const
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec3F(const Vec3F& vec3, int uniformId) const
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, vec3.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) const
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec3UI(const Vec3UI& vec3, int uniformId) const
	{
		GL_CALL(glUniform3uiv, uniformId, 1, vec3.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) const
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformVec2F(const Vec2F& vec2, int uniformId) const
	{
		GL_CALL(glUniform2fv, uniformId, 1, vec2.GetValuePointer());
	}

	void OpenGlComputeShader::SetUniformInt(int value, const std::string& uniformName) const
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void OpenGlComputeShader::SetUniformInt(int value, int uniformId) const
	{
		GL_CALL(glUniform1i, uniformId, value);
	}

	void OpenGlComputeShader::SetUniformFloat(float value, const std::string& uniformName) const
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	void OpenGlComputeShader::SetUniformFloat(float value, int uniformId) const
	{
		GL_CALL(glUniform1f, uniformId, value);
	}

	int OpenGlComputeShader::GetUniformLocation(const std::string& uniformName) const
	{
		return GL_CALL(glGetUniformLocation, programId, uniformName.c_str());
	}

	int OpenGlComputeShader::GetShaderId() const
	{
		return programId;
	}

	const std::string& OpenGlComputeShader::GetShaderName() const
	{
		return name;
	}
}
