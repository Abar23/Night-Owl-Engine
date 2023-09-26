#pragma once

#include "NightOwl/Core/Utitlity/Utils.h"
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>

namespace NightOwl::Utility
{
	class LoggerManager;

	enum class MessageSeverity
	{
		Error,
		Info,
		Assert,
		Debug
	};

	class Logger
	{
	public:
		template <typename... Args>
		void Log(MessageSeverity severity, const std::string &file, int line, const std::string &message, Args&&... args)
		{
			if(logFile.is_open())
			{
				auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
				
				std::string fileName = StripFilePathToName(file);
				
				const std::string preamble = std::format("[{0:%H:%M:%S}] {1}({2}) ", now, fileName, line);

				const int numberOfFormatStringInputs = sizeof...(args);
				const std::string userMessage = numberOfFormatStringInputs == 0 ? message : std::vformat(message, std::make_format_args(args...));

				const std::string severityName = SeverityToString(severity);

				logFile << preamble << severityName << userMessage << std::endl;

				std::cout << preamble << severityName << userMessage << std::endl;
			}
		}

		void Shutdown()
		{
			logFile.close();
		}

	private:
		friend class LoggerManager;

		std::ofstream logFile;
		
		void OpenFile(const char* logFileName);

		static std::string SeverityToString(MessageSeverity severity);
	};
}

