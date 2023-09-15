#pragma once

namespace NightOwl
{
	class ISampler
	{
	public:
		virtual ~ISampler() = default;

		virtual void Bind() = 0;

		virtual void Unbind() = 0;

		virtual void SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) = 0;

		virtual void SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) = 0;

		virtual void SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) = 0;

		virtual void SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) = 0;

		virtual void SetUniformInt(int value, const std::string& uniformName) = 0;

		virtual void SetUniformFloat(float value, const std::string& uniformName) = 0;

		virtual int GetShaderId() = 0;

		virtual const std::string& GetShaderName() = 0;
	};
}

