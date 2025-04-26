#include "NightOwlPch.h"

#define STB_IMAGE_IMPLEMENTATION

#include "AssetManager.h"
#include "AssimpModelLoader.h"
#include "ShaderLoader.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Audio/AudioClip.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Graphics/Types/GraphicsFormat.h"
#include "NightOwl/Graphics/Graphics.h"
#include <filesystem>
#include <stb/stb_image.h>


namespace NightOwl
{
	AssetRepository<IShader>& AssetManager::GetShaderRepository()
	{
		return shaderRepository;
	}

	AssetRepository<IComputeShader>& AssetManager::GetComputeShaderRepository()
	{
		return computeShaderRepository;
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

	AssetRepository<std::string>& AssetManager::GetShaderIncludeRepository()
	{
		return shaderIncludeRepository;
	}

	void* AssetManager::ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels, bool isHdr /* = false */)
	{
		stbi_set_flip_vertically_on_load(true);

		void* data;
		if (isHdr)
		{
			data = stbi_loadf(filePath.c_str(), &width, &height, &numberOfChannels, 0);
		}
		else
		{
			data = stbi_load(filePath.c_str(), &width, &height, &numberOfChannels, 0);
		}
		 
		ENGINE_ASSERT(data != nullptr, "Failed to load texture from file path: {0}", filePath);

		return data;
	}

	void AssetManager::ClearSceneAll()
	{
		shaderRepository.ClearSceneAssets();
		computeShaderRepository.ClearAllAssets();
		textureRepository.ClearSceneAssets();
		audioClipRepository.ClearSceneAssets();
		modelRepository.ClearSceneAssets();
		animationRepository.ClearSceneAssets();
	}

	void AssetManager::ClearAll()
	{
		shaderRepository.ClearAllAssets();
		computeShaderRepository.ClearAllAssets();
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
		const std::string textureName = Utility::StripFilePathToNameWithExtension(filePath);
		if (textureRepository.HasAsset(textureName))
		{
			return textureRepository.GetAsset(textureName);
		}

		int width;
		int height;
		int numberOfChannels;
		GraphicsFormat textureGraphicsFormat;
		void* data;

		const std::string textureExtension = Utility::StripFilePathToExtension(filePath);
		if (textureExtension == "hdr")
		{
			data = ReadTexture2D(filePath, width, height, numberOfChannels, true);

			if (numberOfChannels == 1)
			{
				textureGraphicsFormat = GraphicsFormat::R32F;
			}
			else if (numberOfChannels == 2)
			{
				textureGraphicsFormat = GraphicsFormat::RG32F;
			}
			else if (numberOfChannels == 3)
			{
				textureGraphicsFormat = GraphicsFormat::RGB32F;
			}
			else if (numberOfChannels == 4)
			{
				textureGraphicsFormat = GraphicsFormat::RGBA32F;
			}
		}
		else
		{
			data = ReadTexture2D(filePath, width, height, numberOfChannels);

			if (numberOfChannels == 1)
			{
				textureGraphicsFormat = GraphicsFormat::R8;
			}
			else if (numberOfChannels == 2)
			{
				textureGraphicsFormat = GraphicsFormat::RG8;
			}
			else if (numberOfChannels == 3)
			{
				textureGraphicsFormat = GraphicsFormat::RGB8;
			}
			else if (numberOfChannels == 4)
			{
				textureGraphicsFormat = GraphicsFormat::RGBA8;
			}
		}

		const std::shared_ptr<ITexture2D> texture = Graphics::CreateTexture2D(data, height, width, textureGraphicsFormat);

		textureRepository.AddAsset(textureName, texture, isEngineAsset);

		stbi_image_free(data);

		return texture.get();
	}

	AudioClip* AssetManager::LoadAudioClip(const std::string& filePath, bool isEngineAsset /* = false */)
	{
		const std::string audioClipName = Utility::StripFilePathToNameWithExtension(filePath);

		if (audioClipRepository.HasAsset(audioClipName))
		{
			return audioClipRepository.GetAsset(audioClipName);
		}

		std::shared_ptr<AudioClip> audioClip = std::make_shared<AudioClip>();

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
