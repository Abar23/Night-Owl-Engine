#pragma once

#include <iostream>
#include "AL/alc.h"

#ifdef DEBUG
#define AL_CALL(func, ...) \
	func(__VA_ARGS__); \
	checkOpenAlError(__FILE__, __LINE__)

#define AL_CONTEXT_CALL(func, device, ...) \
	func(__VA_ARGS__); \
	checkOpenAlContextError(__FILE__, __LINE__, device)
#else
#define AL_CALL(func, ...) func(__VA_ARGS__)

#define AL_CONTEXT_CALL(func, device, ...) func(__VA_ARGS__)
#endif

// al.h fucntion call error checker
bool checkOpenAlError(const std::string& filename, const std::uint32_t line);

// alc.h function call error checker
bool checkOpenAlContextError(const std::string& filename, const std::uint32_t line, ALCdevice* device);