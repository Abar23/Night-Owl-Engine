#include <NightOwlPch.h>

#include <iostream>
#include <cerrno>
#include <cstdarg>
#include "Logger.h"


namespace NightOwl::Utility
{
	void Logger::OpenFile(const char* logFileName)
	{
		try
		{
			logFile.exceptions(std::ios::badbit | std::ios::failbit);
			logFile.open(logFileName);
		}
		catch (std::exception& e)
		{
			std::cerr << "Failed to open log file: " << logFileName << std::endl;
			std::cerr << "Exception raised: " << e.what() << std::endl;
			std::cerr << "Error raised: " << strerror(errno) << std::endl;
		}
	}

	std::string Logger::SeverityToString(MessageSeverity severity)
	{
		switch (severity)
		{
		case MessageSeverity::Error:
			return std::string{ "Error: " };

		case MessageSeverity::Assert:
			return std::string{ "Assert: " };

		case MessageSeverity::Debug:
			return std::string{ "Debug: " };

		default:
			return std::string{ "Info: " };
		}
	}
}

