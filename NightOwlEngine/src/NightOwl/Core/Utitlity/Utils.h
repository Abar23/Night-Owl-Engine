#pragma once

#include "NightOwl/Math/Vec3.h"
#include <nlohmann/json.hpp>
#include <string>

namespace NightOwl::Utility
{
	std::string StripFilePathToName(const std::string& filePath);

	std::string StripFilePathToNameWithoutExtension(const std::string& filePath);

	std::string FilePathToDirectory(std::string filePath);

	void StandardizeFilePathString(std::string& filePath);

	nlohmann::json GetJsonDataFromFilePath(const std::string& path);

	Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray);
}
