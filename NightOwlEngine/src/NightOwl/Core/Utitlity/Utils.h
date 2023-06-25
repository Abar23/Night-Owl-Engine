#pragma once

#include "NightOwl/Math/Vec3.h"
#include <nlohmann/json.hpp>
#include <string>

namespace NightOwl::Utility
{
	std::string StripFilePathToName(const std::string& filePath);

	std::string StripFilePathToNameWithoutExtension(const std::string& filePath);

	nlohmann::json GetJsonDataFromFilePath(const std::string& path);

	Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray);

	std::vector<std::vector<std::string>> GetCSVDataFromFilePath(const std::string& path);

	std::vector<std::string> split(const std::string& stream, char delimiter);
}
