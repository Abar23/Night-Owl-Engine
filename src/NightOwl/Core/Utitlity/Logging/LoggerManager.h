#pragma once

#include <memory>
#include "Logger.h"

namespace NightOwl::Utility
{
	class LoggerManager
	{
	public:
		static void Init();

		static std::shared_ptr<Logger> GetEngineLogger();
		static std::shared_ptr<Logger> GetClientLogger();

	private:
		static std::shared_ptr<Logger> engineLogger;
		static std::shared_ptr<Logger> clientLogger;
	};
}

#define ENGINE_LOG_ERROR(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::Error, __FILE__, __LINE__, message, __VA_ARGS__)
#define ENGINE_LOG_INFO(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::Info, __FILE__, __LINE__, message, __VA_ARGS__)
#define ENGINE_LOG_ASSERT(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::Assert, __FILE__, __LINE__, message, __VA_ARGS__)
#define ENGINE_LOG_DEBUG(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::Debug, __FILE__, __LINE__, message, __VA_ARGS__)

#define CLIENT_LOG_ERROR(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::Error, __FILE__, __LINE__, message, __VA_ARGS__)
#define CLIENT_LOG_INFO(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::Info, __FILE__, __LINE__, message, __VA_ARGS__)
#define CLIENT_LOG_ASSERT(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::Assert, __FILE__, __LINE__, message, __VA_ARGS__)
#define CLIENT_LOG_DEBUG(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::Debug, __FILE__, __LINE__, message, __VA_ARGS__)
