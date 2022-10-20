#pragma once

#include <iostream>
#include <format>
#include "Logging/LoggerManager.h"

#ifdef DEBUG
#define ENGINE_ASSERT(condition, message, ...) \
    if (! (condition)) { \
		ENGINE_LOG_ASSERT(message, __VA_ARGS__);\
		std::cerr << "***ENGINE ERROR*** (" << __FILE__ << ": " << __LINE__ << ")\n"; \
        std::cerr << "Assertion `" #condition "` failed! Error message: " << message << std::endl; \
        std::terminate(); \
    }

#define CLIENT_ASSERT(condition, message, ...) \
    if (! (condition)) { \
		CLIENT_LOG_ASSERT(message, __VA_ARGS__);\
		std::cerr << "***CLIENT ERROR*** (" << __FILE__ << ": " << __LINE__ << ")\n"; \
        std::cerr << "Assertion `" #condition "` failed! Error message: " << message << std::endl; \
        std::terminate(); \
    }
#else
#define ENGINE_ASSERT(condition, message)
#define CLIENT_ASSERT(condition, message)
#endif