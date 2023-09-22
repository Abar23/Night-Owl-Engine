#pragma once

#include <unordered_map>
#include <string>
#include <memory>

namespace NightOwl
{
	template<typename T>
	class AssetRepository
	{
	public:
		bool HasAsset(const std::string& assetName)
		{
			if (sceneAssetMap.contains(assetName) || engineAssetMap.contains(assetName))
			{
				return true;
			}

			return false;
		}

		T* GetAsset(const std::string& assetName)
		{
			if (sceneAssetMap.contains(assetName))
			{
				return sceneAssetMap.at(assetName).get();
			}

			if (engineAssetMap.contains(assetName))
			{
				return engineAssetMap.at(assetName).get();
			}

			return nullptr;
		}

		void AddAsset(const std::string& assetName, std::shared_ptr<T> asset, bool isEngineAsset = false)
		{
			if (HasAsset(assetName) == true)
			{
				return;
			}

			if (isEngineAsset)
			{
				engineAssetMap[assetName] = asset;
				return;
			}

			sceneAssetMap[assetName] = asset;
		}

		void RemoveAsset(const std::string& assetName)
		{
			if (sceneAssetMap.contains(assetName))
			{
				sceneAssetMap.erase(assetName);
				return;
			}

			if (engineAssetMap.contains(assetName))
			{
				engineAssetMap.erase(assetName);
			}
		}

		void ClearSceneAssets()
		{
			sceneAssetMap.clear();
		}

		void ClearAllAssets()
		{
			sceneAssetMap.clear();
			engineAssetMap.clear();
		}

	private:
		std::unordered_map<std::string,std::shared_ptr<T>> sceneAssetMap;

		std::unordered_map<std::string, std::shared_ptr<T>> engineAssetMap;
	};
}
