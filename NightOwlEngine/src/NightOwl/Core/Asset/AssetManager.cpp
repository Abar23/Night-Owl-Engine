#include "NightOwlPch.h"

#define STB_IMAGE_IMPLEMENTATION

#include "AssetManager.h"
#include "AssimpModelLoader.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Core/Locator/AudioSystemLocator.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include "stb/stb_image.h"
#include <filesystem>

#include "ShaderLoader.h"

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

	AssetRepository<Animation>& AssetManager::GetAnimationRepository()
	{
		return animationRepository;
	}

	stbi_uc* AssetManager::ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels)
	{
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &numberOfChannels, 4);

		ENGINE_ASSERT(data != nullptr, "Failed to load texture from file path: {0}", filePath);

		return data;
	}

	void AssetManager::ClearSceneAll()
	{
		shaderRepository.ClearSceneAssets();
		textureRepository.ClearSceneAssets();
		audioClipRepository.ClearSceneAssets();
		modelRepository.ClearSceneAssets();
		animationRepository.ClearSceneAssets();
	}

	void AssetManager::ClearAll()
	{
		shaderRepository.ClearAllAssets();
		textureRepository.ClearAllAssets();
		audioClipRepository.ClearAllAssets();
		modelRepository.ClearAllAssets();
		animationRepository.ClearAllAssets();
	}

	void AssetManager::LoadShaders(const std::string& shaderDirectory, bool isEngineAsset /* = false */)
	{
		ShaderLoader::LoadShaders(shaderDirectory, isEngineAsset);
	}

	ITexture2D* AssetManager::LoadTexture2D(const std::string& filePath, bool isEngineAsset /* = false */)
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

		textureRepository.AddAsset(textureName, texture, isEngineAsset);

		stbi_image_free(data);
		
		return texture.get();
	}

	AudioClip* AssetManager::LoadAudioClip(const std::string& filePath, bool isEngineAsset /* = false */)
	{
		const std::string audioClipName = Utility::StripFilePathToName(filePath);

		if (audioClipRepository.HasAsset(audioClipName))
		{
			return audioClipRepository.GetAsset(audioClipName);
		}

		const auto audioClip = std::make_shared<AudioClip>();

		//audioClip->SetSound(sound);

		audioClipRepository.AddAsset(audioClipName, audioClip, isEngineAsset);

		return audioClip.get();
	}

	void AssetManager::LoadModel(const std::string& filePath, bool isEngineAsset /* = false */)
	{
		const std::string modelName = Utility::StripFilePathToNameWithoutExtension(filePath);
		if (modelRepository.HasAsset(modelName))
		{
			return;
		}

		// TODO: return back the model that has been loaded
		// call assimp model load and add mesh to the repository
		AssimpModelLoader::LoadModel(filePath);
	}

	void AssetManager::LoadAnimation(const std::string& filePath, bool isEngineAsset /* = false */)
	{
		const std::string animationName = Utility::StripFilePathToNameWithoutExtension(filePath);
		if (modelRepository.HasAsset(animationName))
		{
			return;
		}

		// TODO: return back the animation that has been loaded
		AssimpModelLoader::LoadAnimation(filePath);
	}

	void AssetManager::LoadEngineAssets()
	{
		// Load all textures
		for (const auto& entry : std::filesystem::recursive_directory_iterator(ENGINE_TEXTURE_DIRECTORY)) {
			if (is_regular_file(entry)) {
				std::string filePath = entry.path().string();
				Utility::StandardizeFilePathString(filePath);
				LoadTexture2D(filePath, true);
			}
		}

		// Load all shaders
		LoadShaders(ENGINE_SHADER_DIRECTORY, true);

		// TODO: add any additional engine specific
	}
}
