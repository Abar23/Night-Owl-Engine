#include "NightOwlPch.h"

#include "Utils.h"
#include "Assert.h"
#include "Logging/LoggerManager.h"

namespace NightOwl::Utility
{
	std::string StripFilePathToName(const std::string& filePath)
	{
		std::size_t delimiterPosition = filePath.rfind('/', filePath.length());
		if(delimiterPosition == std::string::npos)
		{
			// Windows director delimiter
			delimiterPosition = filePath.rfind('\\', filePath.length());
		}

		if(delimiterPosition != std::string::npos)
		{
			return std::string(filePath.substr(delimiterPosition + 1, filePath.length() - delimiterPosition));
		}

		return { "" };
	}

	std::string StripFilePathToNameWithoutExtension(const std::string& filePath)
	{
		std::string fileName = StripFilePathToName(filePath);

		if(fileName.empty())
		{
			return fileName;
		}

		const size_t delimiterPosition = fileName.find_last_of(".");
		if (delimiterPosition == std::string::npos)
		{
			return { "" };
		}

		return fileName.substr(0, delimiterPosition);
	}

	std::string FilePathToDirectory(std::string filePath)
	{
		StandardizeFilePathString(filePath);

		return filePath.substr(0, filePath.find_last_of('/'));
	}

	void StandardizeFilePathString(std::string& filePath)
	{
		std::ranges::replace(filePath, '\\', '/');
	}

	nlohmann::json GetJsonDataFromFilePath(const std::string& path)
	{
		std::ifstream jsonFile(path);
		if (!jsonFile)
		{
			ENGINE_LOG_ERROR("Error to open JSON File : {0}", path);
			std::terminate();
		}

		nlohmann::json jsonDataObject = nlohmann::json::parse(jsonFile);
		jsonFile.close();

		return jsonDataObject;
	}

	Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray)
	{
		ENGINE_ASSERT(dataArray.is_array(), "Expected json array to extract vector from.");

		return { dataArray[0].get<float>(),
				   dataArray[1].get<float>(),
				   dataArray[2].get<float>() };
	}

	Mat4F AssimpMat4ToNightOwlMat4F(const aiMatrix4x4& assimpMatrix)
	{
		Mat4F matrix;

		matrix.SetRow(0, { assimpMatrix.a1, assimpMatrix.a2, assimpMatrix.a3, assimpMatrix.a4 });
		matrix.SetRow(1, { assimpMatrix.b1, assimpMatrix.b2, assimpMatrix.b3, assimpMatrix.b4 });
		matrix.SetRow(2, { assimpMatrix.c1, assimpMatrix.c2, assimpMatrix.c3, assimpMatrix.c4 });
		matrix.SetRow(3, { assimpMatrix.d1, assimpMatrix.d2, assimpMatrix.d3, assimpMatrix.d4 });

		return matrix;
	}

	Vec3F AssimpVec3ToNightOwlVec3F(const aiVector3D& assimpVec3)
	{
		return Vec3F(assimpVec3.x, assimpVec3.y, assimpVec3.z);
	}

	QuatF AssimpQuaternionToNightOwlQuatF(const aiQuaternion& assimpQuaterion)
	{
		return QuatF(assimpQuaterion.x, assimpQuaterion.y, assimpQuaterion.z, assimpQuaterion.w);
	}
}
