#pragma once

#include "glad/glad.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Types/ShaderTypes.h"

namespace NightOwl::Graphics
{
	class OpenGlShader : public IShader
	{
	public:
		OpenGlShader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

		~OpenGlShader() override;

		void Bind() override;

		void Unbind() override;

		void SetUniformMat4F(const Math::Mat4F& mat4, const std::string& uniformName) override;

		void SetUniformVec4F(const Math::Vec4F& vec4, const std::string& uniformName) override;

		void SetUniformVec3F(const Math::Vec3F& vec3, const std::string& uniformName) override;

		void SetUniformVec3UI(const Math::Vec3UI& vec3, const std::string& uniformName) override;

		void SetUniformVec2F(const Math::Vec2F& vec2, const std::string& uniformName) override;

		void SetUniformInt(int value, const std::string& uniformName) override;

		void SetUniformFloat(float value, const std::string& uniformName) override;

		int GetShaderId() override;

		const std::string& GetShaderName() override;

	private:
		unsigned int programId;

		std::string name;

		unsigned int CompileShaderSource(const std::string& shaderSource, GLenum shaderType);

		unsigned int GetUniformLocation(const std::string& uniformName) const;

		static std::string GetShaderSource(const std::string& sourcePath);

	};
}
