#pragma once

#include <string>

namespace NightOwl
{
	class ShaderLoader
	{
	public:
		static void LoadShaders(const std::string& shaderDirectory, bool isEngineAsset = false);

		static void ProcessSharedShaders(const std::string& sharedShaderDirectory);

	private:
		static std::string ReadShader(const std::string& filePath);

		inline static const std::string shaderHeader = R"(
			#version 460 core

			#extension GL_ARB_bindless_texture : require
			#extension GL_ARB_shading_language_include : require

		)";
	};
}
