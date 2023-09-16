#include <NightOwlPch.h>

#include "OpenGLShader.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"

namespace NightOwl
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

		GLint uniformCount;
		glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

		// Loop through all uniform variables
		for (GLuint i = 0; i < static_cast<GLuint>(uniformCount); ++i) {
			GLsizei nameLength;
			GLint size;
			GLenum type;
			GLchar uniformName[256]; // Adjust the size as needed

			// Get information about the current uniform variable
			glGetActiveUniform(programId, i, sizeof(uniformName) - 1,
				&nameLength, &size, &type, uniformName);

			// Ensure the uniform name is null-terminated
			uniformName[nameLength] = '\0';

			// 'uniformName' now contains the name of the uniform variable, and 'type' contains its data type
			// You can print or store this information as needed
			printf("Uniform Name: %s, Type: %u, Location: %d\n", uniformName, type, glGetUniformLocation(programId, uniformName));
		}

		GLint numUniformBlocks;
		glGetProgramiv(programId, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
		for (int i = 0; i < numUniformBlocks; ++i) {
			char blockName[64];
			glGetActiveUniformBlockName(programId, i, sizeof(blockName), NULL, blockName);

			// Use blockName to identify the uniform block.
			GLint blockSize;
			glGetActiveUniformBlockiv(programId, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

			// Get the binding point for the uniform block.
			GLint blockBinding;
			glGetActiveUniformBlockiv(programId, i, GL_UNIFORM_BLOCK_BINDING, &blockBinding);

			printf("Uniform Block %d:\n", i);
			printf("  Name: %s\n", blockName);
			printf("  Size: %d bytes\n", blockSize);
			printf("  Binding Point: %d\n", blockBinding);
		}
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

	void OpenGlShader::SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName)
	{
		GL_CALL(glUniformMatrix4fv, GetUniformLocation(uniformName), 1, false, mat4.GetValuePointer());
	}

	void OpenGlShader::SetUniformMat4F(const Mat4F& mat4, const int uniformId)
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, mat4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName)
	{
		GL_CALL(glUniform4fv, GetUniformLocation(uniformName), 1, vec4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec4F(const Vec4F& vec4, const int uniformId)
	{
		GL_CALL(glUniform4fv, uniformId, 1, vec4.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3fv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3F(const Vec3F& vec3, const int uniformId)
	{
		GL_CALL(glUniformMatrix4fv, uniformId, 1, false, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName)
	{
		GL_CALL(glUniform3uiv, GetUniformLocation(uniformName), 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec3UI(const Vec3UI& vec3, const int uniformId)
	{
		GL_CALL(glUniform3uiv, uniformId, 1, vec3.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName)
	{
		GL_CALL(glUniform2fv, GetUniformLocation(uniformName), 1, vec2.GetValuePointer());
	}

	void OpenGlShader::SetUniformVec2F(const Vec2F& vec2, const int uniformId)
	{
		GL_CALL(glUniform2fv, uniformId, 1, vec2.GetValuePointer());
	}

	void OpenGlShader::SetUniformInt(int value, const std::string& uniformName)
	{
		GL_CALL(glUniform1i, GetUniformLocation(uniformName), value);
	}

	void OpenGlShader::SetUniformInt(const int value, const int uniformId)
	{
		GL_CALL(glUniform1i, uniformId, value);
	}

	void OpenGlShader::SetUniformFloat(float value, const std::string& uniformName)
	{
		GL_CALL(glUniform1f, GetUniformLocation(uniformName), value);
	}

	void OpenGlShader::SetUniformFloat(const float value, const int uniformId)
	{
		GL_CALL(glUniform1f, uniformId, value);
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
		return GL_CALL(glGetUniformLocation, programId, uniformName.c_str());
	}
}
