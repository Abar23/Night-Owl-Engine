#include <NightOwlPch.h>

#include "OpenGLShader.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"

namespace NightOwl
{
	OpenGlShader::OpenGlShader(const std::string& name)
		: name{ name },
		  programId(std::numeric_limits<unsigned int>::max())
	{
		programId = GL_CALL(glCreateProgram);
	}

	OpenGlShader::~OpenGlShader()
	{
		GL_CALL(glUseProgram, 0);
		GL_CALL(glDeleteProgram, programId);
	}

	void OpenGlShader::Bind() const
	{
		GL_CALL(glUseProgram, programId);
	}

	void OpenGlShader::Unbind() const
	{
		GL_CALL(glUseProgram, 0);
	}

	void OpenGlShader::AddShaderStage(const std::shared_ptr<IShaderStage>& shaderStage)
	{
		shaderStages.push_back(shaderStage);
	}

	void OpenGlShader::AttachAndLinkShaderStages()
	{
		for (const auto& shaderStage : shaderStages)
		{
			GL_CALL(glAttachShader, programId, shaderStage->GetId());
		}

		GL_CALL(glLinkProgram, programId);

		CHECK_PROGRAM_LINKER_ERRORS(programId);

		ProcessUniforms();
	}

	void OpenGlShader::SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) const
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, false, mat4.GetValuePointer());
	}

	void OpenGlShader::SetUniformMat4F(const Mat4F& mat4, const int uniformId) const
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, mat4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) const
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec4F(const Vec4F& vec4, const int uniformId) const
	{
		GL_CALL(glUniform4fv, uniformId, 1, vec4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) const
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3F(const Vec3F& vec3, const int uniformId) const
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) const
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3UI(const Vec3UI& vec3, const int uniformId) const
	{
		GL_CALL(glUniform3uiv, uniformId, 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) const
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec2F(const Vec2F& vec2, const int uniformId) const
	{
		GL_CALL(glUniform2fv, uniformId, 1, vec2.GetValuePointer());
	}

	void OpenGlShader::SetUniformInt(int value, const std::string& uniformName) const
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void OpenGlShader::SetUniformInt(const int value, const int uniformId) const
	{
		GL_CALL(glUniform1i, uniformId, value);
	}

	void OpenGlShader::SetUniformFloat(float value, const std::string& uniformName) const
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	void OpenGlShader::SetUniformFloat(const float value, const int uniformId) const
	{
		GL_CALL(glUniform1f, uniformId, value);
	}

	int OpenGlShader::GetShaderId() const
	{
		return programId;
	}

	const std::string& OpenGlShader::GetShaderName() const
	{
		return name;
	}

	const std::array<std::vector<std::pair<std::string, int>>, static_cast<int>(UniformDataTypes::NumberOfTypes)>& OpenGlShader::GetUniformDataMap() const
	{
		return uniformTypeToDataMap;
	}

	void OpenGlShader::ProcessUniforms()
	{
		char bufferName[256];

		// Loop through all uniform variables
		int uniformCount;
		GL_CALL(glGetProgramiv, programId, GL_ACTIVE_UNIFORMS, &uniformCount);
		for (int activeUniformIndex = 0; activeUniformIndex < uniformCount; ++activeUniformIndex) {
			int nameLength;
			int size;
			unsigned int type;

			// Get information about the current uniform variable
			GL_CALL(glGetActiveUniform, programId, activeUniformIndex, sizeof(bufferName) - 1, &nameLength, &size, &type, bufferName);

			// TODO: use size to determine if a uniform is an array of values

			// Ensure the uniform name is null-terminated
			bufferName[nameLength] = '\0';

			const int uniformLocation = GetUniformLocation(bufferName);
			if (uniformLocation == -1)
			{
				continue;
			}

			UniformDataTypes uniformDataType = OpenGlUniformToUniformDataType(type);
			if (uniformDataType == UniformDataTypes::None)
			{
				continue;
			}

			std::pair nameToLocationPair = std::make_pair(bufferName, uniformLocation);

			uniformTypeToDataMap[static_cast<int>(uniformDataType)].emplace_back(nameToLocationPair);
		}

		// Loop through all uniform blocks
		int numUniformBlocks;
		GL_CALL(glGetProgramiv, programId, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
		for (int activeUniformBlockIndex = 0; activeUniformBlockIndex < numUniformBlocks; ++activeUniformBlockIndex) {
			glGetActiveUniformBlockName(programId, activeUniformBlockIndex, sizeof(bufferName), NULL, bufferName);

			// Get the binding point for the uniform block.
			int blockBinding;
			GL_CALL(glGetActiveUniformBlockiv, programId, activeUniformBlockIndex, GL_UNIFORM_BLOCK_BINDING, &blockBinding);

			// UniformDataTypes uniformDataType = UniformDataTypes::Buffer;
			// std::pair nameToLocationPair = std::make_pair(bufferName, blockBinding);
			//
			// uniformTypeToDataMap[static_cast<int>(uniformDataType)].push_back(nameToLocationPair);
		}

		int numBlocks;
		glGetProgramInterfaceiv(programId, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);

		constexpr unsigned int props[] = { GL_BUFFER_BINDING };
		for (int i = 0; i < numBlocks; ++i) 
		{
			GLsizei length;
			glGetProgramResourceName(programId, GL_SHADER_STORAGE_BLOCK, i, sizeof(name), &length, bufferName);

			const GLint blockIndex = glGetProgramResourceIndex(programId, GL_SHADER_STORAGE_BLOCK, bufferName);

			GLint blockBinding;
			glGetProgramResourceiv(programId, GL_SHADER_STORAGE_BLOCK, blockIndex, 1, props, length, nullptr, &blockBinding);

			UniformDataTypes uniformDataType = UniformDataTypes::Buffer;
			std::pair nameToLocationPair = std::make_pair(bufferName, blockBinding);
			uniformTypeToDataMap[static_cast<int>(uniformDataType)].emplace_back(nameToLocationPair);
		}
	}

	int OpenGlShader::GetUniformLocation(const std::string& uniformName) const
	{
		return GL_CALL(glGetUniformLocation, programId, uniformName.c_str());
	}
}
