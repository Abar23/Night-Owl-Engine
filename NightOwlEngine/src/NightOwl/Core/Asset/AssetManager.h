#pragma once

#include "AssetRepository.h"

namespace NightOwl
{
	class Animation;

	class AudioClip;

	class IComputeShader;

	class ITexture2D;

	class IShader;

	class Model;

	class AssetManager
	{
	public:
		AssetRepository<IShader>& GetShaderRepository();

		AssetRepository<IComputeShader>& GetComputeShaderRepository();

		AssetRepository<ITexture2D>& GetTextureRepository();

		AssetRepository<AudioClip>& GetAudioClipRepository();

		AssetRepository<Model>& GetModelRepository();

		AssetRepository<Animation>& GetAnimationRepository();

		AssetRepository<std::string>& GetShaderIncludeRepository();

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

		AssetRepository<IComputeShader> computeShaderRepository;

		AssetRepository<ITexture2D> textureRepository;

		AssetRepository<AudioClip> audioClipRepository;

		AssetRepository<Model> modelRepository;

		AssetRepository<Animation> animationRepository;

		AssetRepository<std::string> shaderIncludeRepository;

		void* ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels, bool isHdr = false);
	};
}