#include "NightOwlPch.h"

#include "ShaderLoader.h"
#include "AssetManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/Graphics.h"
#include <filesystem>


namespace NightOwl
{
	void ShaderLoader::LoadShaders(const std::string& shaderDirectory, bool isEngineAsset /* = false */)
	{
		ProcessSharedShaders(shaderDirectory, isEngineAsset);

		ProcessShaders(shaderDirectory, isEngineAsset);
	}

	void ShaderLoader::ProcessSharedShaders(const std::string& shaderDirectory, bool isEngineAsset)
	{
		const std::string& sharedShaderDirectory = shaderDirectory + "/Shared/";
		if (Utility::IsValidDirectory(sharedShaderDirectory) == false)
		{
			return;
		}

		AssetManager* assetManager = AssetManagerLocator::Get();

		auto& shaderIncludeRepo = assetManager->GetShaderIncludeRepository();

		for (const auto& entry : std::filesystem::directory_iterator(sharedShaderDirectory)) 
		{
			if (is_regular_file(entry) == false) 
			{
				continue;
			}

			std::string filePath = entry.path().string();

			const std::string includeShaderName = Utility::StripFilePathToNameWithExtension(filePath);

			if (shaderIncludeRepo.HasAsset(includeShaderName))
			{
				continue;
			}

			Utility::StandardizeFilePathString(filePath);
			std::string shaderSource = ReadShader(filePath);
			const std::shared_ptr<std::string> shaderSourceAsset = std::make_shared<std::string>(shaderSource);

			shaderIncludeRepo.AddAsset(includeShaderName, shaderSourceAsset, isEngineAsset);
		}
	}

	void ShaderLoader::ProcessShaders(const std::string& shaderDirectory, bool isEngineAsset)
	{
		if (Utility::IsValidDirectory(shaderDirectory) == false)
		{
			return;
		}

		AssetManager* assetManager = AssetManagerLocator::Get();

		for (const auto& shaderProgramDirectory : std::filesystem::directory_iterator(shaderDirectory))
		{
			if (is_directory(shaderProgramDirectory) &&
				shaderProgramDirectory.path().filename() == "Shared")
			{
				continue;
			}

			const std::string shaderName = shaderProgramDirectory.path().filename().string();
			if (assetManager->GetShaderRepository().HasAsset(shaderName))
			{
				continue;
			}

			std::shared_ptr<IShader> shader = Graphics::CreateShader(shaderName);

			for (const auto& shaderStageFile : std::filesystem::directory_iterator(shaderProgramDirectory.path()))
			{
				if (is_regular_file(shaderStageFile) == false)
				{
					continue;
				}

				std::string shaderStageFilePath = shaderStageFile.path().string();
				Utility::StandardizeFilePathString(shaderStageFilePath);

				const std::string& shaderStageSource = shaderHeader + ReadShader(shaderStageFilePath);
				const std::string shaderStageExtension = Utility::StripFilePathToExtension(shaderStageFilePath);

				std::shared_ptr<IShaderStage> shaderStage = Graphics::CreateShaderStage(shaderStageSource, ExtensionToShaderType(shaderStageExtension));

				shader->AddShaderStage(shaderStage);
			}

			shader->AttachAndLinkShaderStages();

			assetManager->GetShaderRepository().AddAsset(shaderName, shader, isEngineAsset);
		}
	}

	std::string ShaderLoader::ReadShader(const std::string& filePath)
	{
		std::string shaderSource;
		try
		{
			std::ifstream shaderSourceFile;

			shaderSourceFile.exceptions(std::ios::failbit | std::ios::badbit);
			shaderSourceFile.open(filePath, std::ios::binary);

			shaderSourceFile.seekg(0, std::ios::end);
			const std::streamsize shaderSourceFileSize = shaderSourceFile.tellg();

			if (shaderSourceFileSize > 0)
			{
				shaderSource.resize(shaderSourceFileSize);
				shaderSourceFile.seekg(0, std::ios::beg);
				shaderSourceFile.read(shaderSource.data(), shaderSourceFileSize);
			}
			else
			{
				ENGINE_LOG_ERROR("Shader source file is empty: {0}", filePath);
				std::terminate();
			}
		}
		catch (std::exception& exception)
		{
			ENGINE_LOG_ERROR("Failed to open shader source file: {0}\n Exception raised: {1}\n, Error raised: {2}", filePath, exception.what(), strerror(errno));
			std::terminate();
		}

		return shaderSource;
	}
}
