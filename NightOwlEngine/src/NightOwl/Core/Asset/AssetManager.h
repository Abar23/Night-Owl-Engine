#pragma once

#include "AssetRepository.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Audio/AudioClip.h"

typedef unsigned char stbi_uc;

namespace NightOwl
{
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