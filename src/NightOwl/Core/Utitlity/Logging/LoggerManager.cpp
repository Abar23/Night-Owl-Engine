#include <filesystem>
#include "LoggerManager.h"

namespace NightOwl::Utility
{
	std::shared_ptr<Logger> LoggerManager::clientLogger = std::make_shared<Logger>();
	std::shared_ptr<Logger> LoggerManager::engineLogger = std::make_shared<Logger>();

	void LoggerManager::Init()
	{
		std::filesystem::create_directory(LOG_FILE_DIR);
		clientLogger->OpenFile(CLIENT_LOG_FILE);
		engineLogger->OpenFile(ENGINE_LOG_FILE);
	}

	std::shared_ptr<Logger> LoggerManager::GetEngineLogger()
	{
		return engineLogger;
	}

	std::shared_ptr<Logger> LoggerManager::GetClientLogger()
	{
		return clientLogger;
	}
}
