#pragma once

#include "NightOwl/Core/Asset/AssetManager.h"

namespace NightOwl
{
	class AssetManagerLocator
	{
	public:
		static AssetManager* GetAssetManager()
		{
			return assetManagerInstance;
		}

		static void Provide(AssetManager* assetManager)
		{
			assetManagerInstance = assetManager;
		}

	private:
		inline static AssetManager* assetManagerInstance{ nullptr };
	};
}
