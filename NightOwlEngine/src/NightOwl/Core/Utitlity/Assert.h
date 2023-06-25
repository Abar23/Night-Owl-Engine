#pragma once

#include <iostream>
#include <format>
#include <string>
#include "Logging/LoggerManager.h"
#include "NightOwl/Core/Utitlity/Utils.h"

#ifdef DEBUG
#define ENGINE_ASSERT(condition, message, ...) \
    do \
	if (! (condition)) { \
		std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
		formattedMessage += std::format(message, __VA_ARGS__); \
		ENGINE_LOG_ASSERT(formattedMessage);\
        std::terminate(); \
    } \
    while(0)

#define CLIENT_ASSERT(condition, message, ...) \
    do \
	if (! (condition)) { \
		std::string formattedMessage = std::format("Condition \"{0}\" failed! ", #condition); \
		formattedMessage += std::format(message, __VA_ARGS__); \
		CLIENT_LOG_ASSERT(formattedMessage);\
        std::terminate(); \
    } \
    while(0)
#else
#define ENGINE_ASSERT(condition, message)
#define CLIENT_ASSERT(condition, message)
#endif
