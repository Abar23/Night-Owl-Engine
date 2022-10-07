#pragma once

#include <string>
#include <chrono>
#include <fstream>

namespace NightOwl::Utility
{
	enum MessageSeverity
	{
		ERROR,
		INFO
	};

	class Logger
	{
	public:
		Logger(const char* logFileName);

		template <typename... Args>
		void Log(MessageSeverity severity, const char* file, int line, const char* message, Args&&... args)
		{
			if(logFile.is_open())
			{
				auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
				std::string formatted_time = std::format("[{0:%H:%M:%S}] {1}({2}) ", now, file, line);
				std::string userFormatString = std::vformat(message, std::make_format_args(args...));
				std::string severityMessage = SeverityToString(severity);
				logFile << formatted_time << severityMessage << userFormatString << std::endl;
			}
		}

	private:

		std::ofstream logFile;

		void OpenFile(const char* logFileName);

		static std::string SeverityToString(MessageSeverity severity);
	};
}

#ifdef _WIN64
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define ENGINE_LOG_INFO(message, ...)     l.Log(NightOwl::Utility::INFO, __FILENAME__, __LINE__, message, __VA_ARGS__)

