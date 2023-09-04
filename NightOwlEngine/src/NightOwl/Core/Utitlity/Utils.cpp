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
}
