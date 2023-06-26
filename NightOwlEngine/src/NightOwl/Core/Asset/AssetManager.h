#pragma once

#include "AssetRepository.h"
#include "NightOwl/Graphics/Interfaces/IShader.h"
#include "NightOwl/Graphics/Interfaces/ITexture2D.h"
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

		IShader* LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		ITexture2D* LoadTexture2D(std::string filePath);

		AudioClip* LoadAudioClip(std::string filePath);

		void ClearAll();

	private:
		AssetRepository<IShader> shaderRepository;

		AssetRepository<ITexture2D> textureRepository;

		AssetRepository<AudioClip> audioClipRepository;

		stbi_uc* ReadTexture2D(const std::string& filePath, int& width, int& height, int& numberOfChannels);

		std::string ReadShader(const std::string& filePath);
	};
}