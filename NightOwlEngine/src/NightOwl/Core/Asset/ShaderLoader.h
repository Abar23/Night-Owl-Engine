#pragma once

#include <string>

namespace NightOwl
{
	class ShaderLoader
	{
	public:
		static void LoadShaders(const std::string& shaderDirectory, bool isEngineAsset = false);

		static void ProcessSharedShaders(const std::string& sharedShaderDirectory, bool isEngineAsset);

		static void ProcessShaders(const std::string& shaderDirectory, bool isEngineAsset);

	private:
		static std::string ReadShader(const std::string& filePath);

		inline static const std::string shaderHeader = R"(
			#version 450 core
		)";
	};
}
