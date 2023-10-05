#pragma once

namespace NightOwl
{
	class AssetManager;
	
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
