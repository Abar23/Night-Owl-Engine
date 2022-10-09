#pragma once

#include <string>
#include <cstdint>
#include "glad/glad.h"

#ifdef DEBUG

#define GL_CALL(func, ...) \
	func(__VA_ARGS__); \
	CheckOpenGlError(__FILE__, __LINE__)

#define CHECK_SHADER_COMPILER_ERRORS(shaderId) \
    CheckOpenGlShaderCompilerErrors(shaderId)

#define CHECK_PROGRAM_LINKER_ERRORS(programId) \
    CheckOpenGlShaderProgramLinkerErrors(programId)

#else

#define GL_CALL(func, ...) func(__VA_ARGS__)

#define CHECK_SHADER_COMPILER_ERRORS(shaderId)

#define CHECK_PROGRAM_LINKER_ERRORS(programId)

#endif

bool CheckOpenGlError(const std::string& filename, const std::uint32_t line);

void APIENTRY GlDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

void CheckOpenGlShaderCompilerErrors(const unsigned int shaderId);

void CheckOpenGlShaderProgramLinkerErrors(const unsigned int shaderId);