#pragma once

#include <map>

#include "glad/glad.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Types/ShaderTypes.h"
#include "NightOwl/Graphics/Types/UniformDataTypes.h"

namespace NightOwl
{
	class OpenGlShader : public IShader
	{
	public:
		OpenGlShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		~OpenGlShader() override;

		void Bind() override;

		void Unbind() override;

		void SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) override;

		void SetUniformMat4F(const Mat4F& mat4, const int uniformId) override;

		void SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) override;

		void SetUniformVec4F(const Vec4F& vec4, const int uniformId) override;

		void SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) override;

		void SetUniformVec3F(const Vec3F& vec4, const int uniformId) override;

		void SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) override;

		void SetUniformVec3UI(const Vec3UI& vec3, const int uniformId) override;

		void SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) override;

		void SetUniformVec2F(const Vec2F& vec2, const int uniformId) override;

		void SetUniformInt(int value, const std::string& uniformName) override;

		void SetUniformInt(const int value, const int uniformId) override;

		void SetUniformFloat(float value, const std::string& uniformName) override;

		void SetUniformFloat(const float value, const int uniformId) override;

		int GetUniformLocation(const std::string& uniformName) const override;

		int GetShaderId() override;

		const std::string& GetShaderName() override;

		const std::array<std::vector<std::pair<std::string, int>>, static_cast<int>(UniformDataTypes::NumberOfTypes)>& GetUniformDataMap() override;

	private:
		unsigned int programId;

		std::string name;

		std::array<std::vector<std::pair<std::string, int>>, static_cast<int>(UniformDataTypes::NumberOfTypes)> uniformTypeToDataMap;

		unsigned int CompileShaderSource(const std::string& shaderSource, GLenum shaderType);

		void ProcessUniforms();
	};
}
