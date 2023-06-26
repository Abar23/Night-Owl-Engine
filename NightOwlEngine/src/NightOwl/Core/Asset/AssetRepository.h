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
		bool HasAsset(std::string assetName)
		{
			return assetMap.contains(assetName);
		}

		T* GetAsset(std::string assetName)
		{
			return assetMap.at(assetName).get();
		}

		void AddAsset(std::string assetName, std::shared_ptr<T> asset)
		{
			if (!HasAsset(assetName))
			{
				assetMap[assetName] = asset;
			}
		}

		void RemoveAsset(std::string assetName)
		{
			assetMap.erase(assetName);
		}

		void ClearAssets()
		{
			assetMap.clear();
		}

	private:
		std::unordered_map<std::string,std::shared_ptr<T>> assetMap;
	};
}
