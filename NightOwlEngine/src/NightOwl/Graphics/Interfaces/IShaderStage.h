#pragma once

#include "NightOwl/Graphics/Types/ShaderType.h"

namespace NightOwl
{
	class IShaderStage
	{
	public:
		virtual ~IShaderStage() = default;

		virtual unsigned int GetId() = 0;

		virtual ShaderType GetType() = 0;

		virtual void SetShaderSource(const std::string& shaderSource, ShaderType shaderType) = 0;
	};
}
