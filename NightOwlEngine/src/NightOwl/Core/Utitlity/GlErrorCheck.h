#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <string>

#ifdef DEBUG

#define GL_CALL(func, ...) \
	func(__VA_ARGS__); \
	NightOwl::Utility::CheckOpenGlError(__FILE__, __LINE__)

#define CHECK_SHADER_COMPILER_ERRORS(shaderId) \
    NightOwl::Utility::CheckOpenGlShaderCompilerErrors(shaderId)

#define CHECK_PROGRAM_LINKER_ERRORS(programId) \
    NightOwl::Utility::CheckOpenGlShaderProgramLinkerErrors(programId)

#define CHECK_FRAME_BUFFER_COMPLETENESS(frameBufferId) \
    NightOwl::Utility::CheckFrameBufferCompleteness(frameBufferId)

#else

#define GL_CALL(func, ...) func(__VA_ARGS__)

#define CHECK_SHADER_COMPILER_ERRORS(shaderId)

#define CHECK_PROGRAM_LINKER_ERRORS(programId)

#define CHECK_FRAME_BUFFER_COMPLETENESS(frameBufferId)

#endif

namespace NightOwl::Utility
{
    bool CheckOpenGlError(const std::string& filename, std::uint32_t line);

    void APIENTRY GlDebugOutput(GLenum source,
        GLenum type,
        unsigned int id,
        GLenum severity,
        GLsizei length,
        //these two * Goes into GLcalls 
        const char* message,
        const void* userParam);

    void CheckOpenGlShaderCompilerErrors(unsigned int shaderId);

    void CheckOpenGlShaderProgramLinkerErrors(unsigned int shaderId);

    void CheckFrameBufferCompleteness(unsigned int frameBufferId);
}