#pragma once

#include "AssetRepository.h"

typedef unsigned char stbi_uc;

namespace NightOwl
{
	class Animation;

	class AudioClip;

	class ITexture2D;

	class IShader;

	class Model;

	class AssetManager
	{
	public:
		AssetRepository<IShader>& GetShaderRepository();

		AssetRepository<ITexture2D>& GetTextureRepository();

		AssetRepository<AudioClip>& GetAudioClipRepository();

		AssetRepository<Model>& GetModelRepository();

		AssetRepository<Animation>& GetAnimationRepository();

		void LoadShaders(const std::string& shaderDirectory, bool isEngineAsset = false);

		ITexture2D* LoadTexture2D(const std::string& filePath, bool isEngineAsset = false);

		AudioClip* LoadAudioClip(const std::string& filePath, bool isEngineAsset = false);

		void LoadModel(const std::string& filePath, bool isEngineAsset = false);

		void LoadAnimation(const std::string& filePath, bool isEngineAsset = false);

		void LoadEngineAssets();

		void ClearSceneAll();

		void ClearAll();

	private:
		AssetRepository<IShader> shaderRepository;

		AssetRepository<ITexture2D> textureRepository;

		AssetRepository<AudioClip> audioClipRepository;

		AssetRepository<Model> modelRepository;

		AssetRepository<Animation> animationRepository;

		stbi_uc* ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels);
	};
}