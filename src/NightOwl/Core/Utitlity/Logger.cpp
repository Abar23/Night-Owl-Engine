#include <iostream>
#include <cerrno>
#include <format>
#include <cstdarg>
#include "Logger.h"


namespace NightOwl::Utility
{
	Logger::Logger(const char* logFileName)
	{
		OpenFile(logFileName);
	}

	void Logger::OpenFile(const char* logFileName)
	{
		try
		{
			logFile.exceptions(std::ios::badbit | std::ios::failbit);
			logFile.open(logFileName);
		}
		catch (std::exception& e)
		{
			std::cerr << "Failed to open log file!";
			std::cerr << "Exception raised: " << e.what();
			std::cerr << "Error raised: " << strerror(errno);
		}
	}

	std::string Logger::SeverityToString(MessageSeverity severity)
	{
		switch (severity)
		{
		case ERROR:
			return std::string{ "ERROR: " };
		default:
			return std::string{ "INFO: " };
		}
	}
}

