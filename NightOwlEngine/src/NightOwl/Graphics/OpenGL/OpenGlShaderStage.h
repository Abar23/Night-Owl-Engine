#pragma once

#include "NightOwl/Graphics/Interfaces/IShaderStage.h"
#include <string>

namespace NightOwl
{
	class OpenGlShaderStage : public IShaderStage
	{
	public:
		OpenGlShaderStage();

		OpenGlShaderStage(const std::string& shaderSource, ShaderType shaderType);

		~OpenGlShaderStage() override;

		unsigned int GetId() override;

		ShaderType GetType() override;

		void SetShaderSource(const std::string& shaderSource, ShaderType shaderType) override;

	private:
		unsigned int shaderId;

		ShaderType shaderType;

		void CompileShaderSource(const std::string& shaderSource);

		void DeleteShaderStage() const;
	};
}
