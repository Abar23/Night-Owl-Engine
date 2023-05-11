#include "NightOwlPch.h"

#include "Utils.h"
#include "Assert.h"
#include "Logging/LoggerManager.h"
#include <fast-cpp-csv-parser/csv.h>

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

	Math::Vec3F GetVec3fFromJsonArray(const nlohmann::json& dataArray)
	{
		ENGINE_ASSERT(dataArray.is_array(), "Expected json array to extract vector from.");

		return { dataArray[0].get<float>(),
				   dataArray[1].get<float>(),
				   dataArray[2].get<float>() };
	}

	std::vector<std::vector<std::string>> GetCSVDataFromFilePath(const std::string& path)
	{
		std::vector<std::vector<std::string>> csvData;
		io::LineReader lineReader(path);

		while (auto line = lineReader.next_line())
		{
			auto splitLine = split(line, ',');

			std::vector<std::string> lineVector;
			for (auto& word : splitLine)
			{
				lineVector.push_back(word);
			}
			csvData.push_back(lineVector);
		}

		return csvData;
	}

	std::vector<std::string> split(const std::string& stream, const char delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(stream);
		while (std::getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}
}
