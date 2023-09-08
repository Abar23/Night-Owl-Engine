#pragma once

#include "AssetRepository.h"
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

		IShader* LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		ITexture2D* LoadTexture2D(const std::string& filePath);

		AudioClip* LoadAudioClip(const std::string& filePath);

		void LoadModel(const std::string& filePath);

		void ClearAll();

	private:
		AssetRepository<IShader> shaderRepository;

		AssetRepository<ITexture2D> textureRepository;

		AssetRepository<AudioClip> audioClipRepository;

		AssetRepository<Model> modelRepository;

		stbi_uc* ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels);

		std::string ReadShader(const std::string& filePath);
	};
}