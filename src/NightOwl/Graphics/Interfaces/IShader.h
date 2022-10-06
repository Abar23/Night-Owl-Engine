#pragma once

#include "NightOwl/Math/Math.h"

namespace NightOwl::Graphics
{
	enum ShaderType
	{
		VERT,
		TESS_CONTROL,
		TESS_EVAL,
		GEOM,
		FRAG
	};

	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetUniformMat4F(const Math::Mat4F& mat4, const std::string& uniformName) = 0;
		virtual void SetUniformVec4F(const Math::Vec4F& vec4, const std::string& uniformName) = 0;
		virtual void SetUniformVec3F(const Math::Vec3F &vec3, const std::string &uniformName) = 0;
		virtual void SetUniformVec3UF(const Math::Vec3UF& vec3, const std::string& uniformName) = 0;
		virtual void SetUniformVec2F(const Math::Vec2F &vec2, const std::string& uniformName) = 0;
		virtual void SetUniformInt(const int id, const std::string& uniformName) = 0;

		virtual int GetShaderId() = 0;
		virtual const std::string& GetShaderName() = 0;
		virtual ShaderType GetShaderType() = 0;
	};
}
