#pragma once

#include "NightOwl/Math/Math.h"
#include <nlohmann/json.hpp>
#include <string>
#include <assimp/matrix4x4.h>

namespace NightOwl::Utility
{
	std::string StripFilePathToName(const std::string& filePath);

	std::string StripFilePathToNameWithoutExtension(const std::string& filePath);

	std::string FilePathToDirectory(std::string filePath);

	void StandardizeFilePathString(std::string& filePath);

	nlohmann::json GetJsonDataFromFilePath(const std::string& path);

	Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray);

	Mat4F AssimpMat4ToNightOwlMat4(const aiMatrix4x4& assimpMatrix);
}
