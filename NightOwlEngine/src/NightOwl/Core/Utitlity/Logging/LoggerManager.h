#pragma once

#include <memory>
#include "Logger.h"

namespace NightOwl::Utility
{
	class LoggerManager
	{
	public:
		static void Init();

		static void Shutdown();

		static std::shared_ptr<Logger> GetEngineLogger();

		static std::shared_ptr<Logger> GetClientLogger();

	private:
		static std::shared_ptr<Logger> engineLogger;

		static std::shared_ptr<Logger> clientLogger;
	};
}

#ifdef DEBUG
	#define ENGINE_LOG_ERROR(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::MessageSeverity::Error, __FILE__, __LINE__, message, __VA_ARGS__)
	#define ENGINE_LOG_INFO(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::MessageSeverity::Info, __FILE__, __LINE__, message, __VA_ARGS__)
	#define ENGINE_LOG_ASSERT(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::MessageSeverity::Assert, __FILE__, __LINE__, message, __VA_ARGS__)
	#define ENGINE_LOG_DEBUG(message, ...)	NightOwl::Utility::LoggerManager::GetEngineLogger()->Log(NightOwl::Utility::MessageSeverity::Debug, __FILE__, __LINE__, message, __VA_ARGS__)
#else 
	#define ENGINE_LOG_ERROR(message, ...)
	#define ENGINE_LOG_INFO(message, ...)
	#define ENGINE_LOG_ASSERT(message, ...)
	#define ENGINE_LOG_DEBUG(message, ...)
#endif

#ifdef DEBUG
	#define CLIENT_LOG_ERROR(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::MessageSeverity::Error, __FILE__, __LINE__, message, __VA_ARGS__)
	#define CLIENT_LOG_INFO(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::MessageSeverity::Info, __FILE__, __LINE__, message, __VA_ARGS__)
	#define CLIENT_LOG_ASSERT(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::MessageSeverity::Assert, __FILE__, __LINE__, message, __VA_ARGS__)
	#define CLIENT_LOG_DEBUG(message, ...)	NightOwl::Utility::LoggerManager::GetClientLogger()->Log(NightOwl::Utility::MessageSeverity::Debug, __FILE__, __LINE__, message, __VA_ARGS__)
#else 
	#define CLIENT_LOG_ERROR(message, ...)
	#define CLIENT_LOG_INFO(message, ...)
	#define CLIENT_LOG_ASSERT(message, ...)
	#define CLIENT_LOG_DEBUG(message, ...)
#endif
