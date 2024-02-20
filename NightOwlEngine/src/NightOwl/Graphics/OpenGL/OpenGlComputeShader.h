#pragma once

#include "NightOwl/Graphics/Interfaces/IComputeShader.h"

namespace NightOwl
{
	class OpenGlComputeShader : public IComputeShader
	{
	public:
		OpenGlComputeShader(const std::string& name);

		~OpenGlComputeShader() override;

		void Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ) const override;

		void Bind() const override;

		void Unbind() const override;

		void AttachAndLinkComputeShaderStage(const std::shared_ptr<IShaderStage>& shaderStage) const override;

		void SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) const override;

		void SetUniformMat4F(const Mat4F& mat4, int uniformId) const override;

		void SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) const override;

		void SetUniformVec4F(const Vec4F& vec4, int uniformId) const override;

		void SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) const override;

		void SetUniformVec3F(const Vec3F& vec3, int uniformId) const override;

		void SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) const override;

		void SetUniformVec3UI(const Vec3UI& vec3, int uniformId) const override;

		void SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) const override;

		void SetUniformVec2F(const Vec2F& vec2, int uniformId) const override;

		void SetUniformInt(int value, const std::string& uniformName) const override;

		void SetUniformInt(int value, int uniformId) const override;

		void SetUniformFloat(float value, const std::string& uniformName) const override;

		void SetUniformFloat(float value, int uniformId) const override;

		int GetUniformLocation(const std::string& uniformName) const override;

		int GetShaderId() const override;

		const std::string& GetShaderName() const override;

	private:
		std::string name;

		unsigned int programId;
	};
}
