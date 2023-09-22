#pragma once

#include "NightOwl/Math/Math.h"
#include <nlohmann/json.hpp>
#include <string>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/types.h>

namespace NightOwl::Utility
{
	std::string StripFilePathToName(const std::string& filePath);

	std::string StripFilePathToNameWithoutExtension(const std::string& filePath);

	std::string StripFilePathToExtension(const std::string& filePath);

	std::string FilePathToDirectory(std::string filePath);

	void StandardizeFilePathString(std::string& filePath);

	nlohmann::json GetJsonDataFromFilePath(const std::string& path);

	Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray);

	Mat4F AssimpMat4ToNightOwlMat4F(const aiMatrix4x4& assimpMatrix);

	Vec3F AssimpVec3ToNightOwlVec3F(const aiVector3D& assimpVec3);

	Vec4F AssimpColor3DToNightOwlVec4F(const aiColor3D& assimpColor);

	QuatF AssimpQuaternionToNightOwlQuatF(const aiQuaternion& assimpQuaterion);
}
