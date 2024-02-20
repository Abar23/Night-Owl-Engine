#pragma once

#include "NightOwl/Math/Mat4.h"

namespace NightOwl
{
	class IShaderStage;

	class IComputeShader
	{
	public:
		virtual ~IComputeShader() = default;

		virtual void Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ) const = 0;

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual void AttachAndLinkComputeShaderStage(const std::shared_ptr<IShaderStage>& shaderStage) const = 0;

		virtual void SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) const = 0;

		virtual void SetUniformMat4F(const Mat4F& mat4, int uniformId) const = 0;

		virtual void SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) const = 0;

		virtual void SetUniformVec4F(const Vec4F& vec4, int uniformId) const = 0;

		virtual void SetUniformVec3F(const Vec3F& vec3, const std::string& uniformName) const = 0;

		virtual void SetUniformVec3F(const Vec3F& vec3, int uniformId) const = 0;

		virtual void SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) const = 0;

		virtual void SetUniformVec3UI(const Vec3UI& vec3, int uniformId) const = 0;

		virtual void SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) const = 0;

		virtual void SetUniformVec2F(const Vec2F& vec2, int uniformId) const = 0;

		virtual void SetUniformInt(int value, const std::string& uniformName) const = 0;

		virtual void SetUniformInt(int value, int uniformId) const = 0;

		virtual void SetUniformFloat(float value, const std::string& uniformName) const = 0;

		virtual void SetUniformFloat(float value, int uniformId) const = 0;

		virtual int GetUniformLocation(const std::string& uniformName) const = 0;

		virtual int GetShaderId() const = 0;

		virtual const std::string& GetShaderName() const = 0;
	};
}
