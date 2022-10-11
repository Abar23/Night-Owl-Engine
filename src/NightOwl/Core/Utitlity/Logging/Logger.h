#pragma once

#include <string>
#include <chrono>
#include <fstream>

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
				
				#ifdef _WIN64
				const std::string fileName = (strrchr(file.c_str(), '\\') ? strrchr(file.c_str(), '\\') + 1 : file.c_str());
				#else
				std::string fileName = (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__);
				#endif
				
				const std::string time = std::format("[{0:%H:%M:%S}] {1}({2}) ", now, fileName, line);

				const std::string userMessage = std::vformat(message, std::make_format_args(args...));

				const std::string severityName = SeverityToString(severity);

				logFile << time << severityName << userMessage << std::endl;
			}
		}

	private:
		friend class LoggerManager;

		std::ofstream logFile;

		void OpenFile(const char* logFileName);

		static std::string SeverityToString(MessageSeverity severity);
	};
}

