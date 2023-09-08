#include "NightOwlPch.h"

#define STB_IMAGE_IMPLEMENTATION

#include "AssetManager.h"

#include "AssimpModelLoader.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include "stb/stb_image.h"

namespace NightOwl
{
	AssetRepository<IShader>& AssetManager::GetShaderRepository()
	{
		return shaderRepository;
	}

	AssetRepository<ITexture2D>& AssetManager::GetTextureRepository()
	{
		return textureRepository;
	}

	AssetRepository<AudioClip>& AssetManager::GetAudioClipRepository()
	{
		return audioClipRepository;
	}

	AssetRepository<Model>& AssetManager::GetModelRepository()
	{
		return modelRepository;
	}

	stbi_uc* AssetManager::ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels)
	{
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &numberOfChannels, 4);

		ENGINE_ASSERT(data != nullptr, "Failed to load texture from file path: {0}", filePath);

		return data;
	}

	std::string AssetManager::ReadShader(const std::string& filePath)
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
		catch (std::exception& e)
		{
			ENGINE_LOG_ERROR("Failed to open shader source file: {0}\n Exception raised: {1}\n, Error raised: {2}", filePath, e.what(), strerror(errno));
			std::terminate();
		}

		return shaderSource;
	}

	void AssetManager::ClearAll()
	{
		shaderRepository.ClearAssets();
		textureRepository.ClearAssets();
		audioClipRepository.ClearAssets();
	}

	IShader* AssetManager::LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		if (shaderRepository.HasAsset(name))
		{
			return shaderRepository.GetAsset(name);
		}

		std::string vertexShaderSource = ReadShader(vertexShaderPath);
		std::string fragmentShaderSource = ReadShader(fragmentShaderPath);

		const auto shader = RenderApi::CreateShader(name, vertexShaderSource, fragmentShaderSource);

		shaderRepository.AddAsset(name, shader);

		return shader.get();
	}

	ITexture2D* AssetManager::LoadTexture2D(const std::string& filePath)
	{
		const std::string textureName = Utility::StripFilePathToName(filePath);

		if (textureRepository.HasAsset(textureName))
		{
			return textureRepository.GetAsset(textureName);
		}

		int width;
		int height;
		int numberOfChannels;

		stbi_uc* data = ReadTexture2D(filePath, width, height, numberOfChannels);

		const auto texture = RenderApi::CreateTexture2D(data, height, width, numberOfChannels);

		textureRepository.AddAsset(textureName, texture);

		stbi_image_free(data);
		
		return texture.get();
	}

	AudioClip* AssetManager::LoadAudioClip(const std::string& filePath)
	{
		//FMOD::Sound* sound = AudioSystemLocator::GetAudioSystem()->CreateSound(filePath, FMOD_DEFAULT).GetPointer();

		//ENGINE_ASSERT(sound != nullptr, "Failed to load sound from file path: {0}", filePath);

		const std::string audioClipName = Utility::StripFilePathToName(filePath);

		if (audioClipRepository.HasAsset(audioClipName))
		{
			return audioClipRepository.GetAsset(audioClipName);
		}

		const auto audioClip = std::make_shared<AudioClip>();

		//audioClip->SetSound(sound);

		audioClipRepository.AddAsset(audioClipName, audioClip);

		return audioClip.get();
	}

	void AssetManager::LoadModel(const std::string& filePath)
	{
		// call assimp model load and add mesh to the repository
		AssimpModelLoader::LoadModel(filePath);
	}
}
