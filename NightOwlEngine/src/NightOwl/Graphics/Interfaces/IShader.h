#pragma once

#include "IShaderStage.h"
#include "NightOwl/Graphics/Types/UniformDataTypes.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;

		virtual void Unbind() const = 0;

		virtual void AddShaderStage(const std::shared_ptr<IShaderStage>& shaderStage) = 0;

		virtual void AttachAndLinkShaderStages() = 0;

		virtual void SetUniformMat4F(const Mat4F& mat4, const std::string& uniformName) const = 0;

		virtual void SetUniformMat4F(const Mat4F& mat4, const int uniformId) const = 0;

		virtual void SetUniformVec4F(const Vec4F& vec4, const std::string& uniformName) const = 0;

		virtual void SetUniformVec4F(const Vec4F& vec4, const int uniformId) const = 0;

		virtual void SetUniformVec3F(const Vec3F &vec3, const std::string& uniformName) const = 0;

		virtual void SetUniformVec3F(const Vec3F& vec3, const int uniformId) const = 0;

		virtual void SetUniformVec3UI(const Vec3UI& vec3, const std::string& uniformName) const = 0;

		virtual void SetUniformVec3UI(const Vec3UI& vec3, const int uniformId) const = 0;

		virtual void SetUniformVec2F(const Vec2F& vec2, const std::string& uniformName) const = 0;

		virtual void SetUniformVec2F(const Vec2F& vec2, const int uniformId) const = 0;

		virtual void SetUniformInt(int value, const std::string& uniformName) const = 0;

		virtual void SetUniformInt(const int value, const int uniformId) const = 0;

		virtual void SetUniformFloat(float value, const std::string& uniformName) const = 0;

		virtual void SetUniformFloat(const float value, const int uniformId) const = 0;

		virtual int GetUniformLocation(const std::string& uniformName) const = 0;

		virtual int GetShaderId() const = 0;

		virtual const std::string& GetShaderName() const = 0;

		virtual const std::array<std::vector<std::pair<std::string, int>>, static_cast<int>(UniformDataTypes::NumberOfTypes)>& GetUniformDataMap() const = 0;
	};
}
