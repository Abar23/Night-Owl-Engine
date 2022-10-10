#include <iostream>
#include "GlErrorCheck.h"
#include "Logging/LoggerManager.h"

bool CheckOpenGlError(const std::string& filename, const std::uint32_t line)
{
	GLenum glErrorCode = glGetError();
	if (glErrorCode != GL_NO_ERROR)
	{
		std::string errorMessage;
		errorMessage += filename;
		errorMessage += " ";
		errorMessage += line;
		switch (glErrorCode)
		{
		case GL_INVALID_ENUM:
			errorMessage += "GL_INVALID_ENUM: an unacceptable value has been passed for an enumerated argument in an OpenGL function.";
			break;

		case GL_INVALID_VALUE:
			errorMessage += "GL_INVALID_VALUE: an out of range numeric argument was passed to an OpenGL function.";
			break;

		case GL_INVALID_OPERATION:
			errorMessage += "GL_INVALID_OPERATION: an OpenGL function that is not allowed in the current OpenGL state was called.";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errorMessage += "GL_INVALID_FRAMEBUFFER_OPERATION: an OpenGL framer buffer object is not complete.";
			break;

		case GL_OUT_OF_MEMORY:
			errorMessage += "GL_OUT_OF_MEMORY: not enough memory is left to complete the called OpenGL function.";
			break;

		case GL_STACK_UNDERFLOW:
			errorMessage += "GL_STACK_UNDERFLOW: an OpenGL pop operation was called at the bottom of the OpenGL stack.";
			break;

		case GL_STACK_OVERFLOW:
			errorMessage += "GL_STACK_OVERFLOW: an OpenGL push operation has exceeded the maximum size of the OpenGl stack.";
			break;
		}
		ENGINE_LOG_ERROR(errorMessage);
		return false;
	}
	return true;
}

void GlDebugOutput(GLenum source, 
				   GLenum type, 
				   unsigned int id, 
				   GLenum severity, 
				   GLsizei length, 
				   const char* message, 
				   const void* userParam)
{
	if (id == 131185 || id == 131218) return;

	std::string debugMessage("OpenGL debug message\n. Source is ");
	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		debugMessage += "OpenGL API.";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		debugMessage += "Windowing System API.";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		debugMessage += "Shader Compiler.";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		debugMessage += "Third Party Application associated with OpenGL.";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		debugMessage += "User of this Application.";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		debugMessage += "Other.";
		break;
	}

	debugMessage += "\nType is ";
	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		debugMessage += "API Error.";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		debugMessage += "Deprecated Behavior Used.";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		debugMessage += "Undefined Behavior.";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		debugMessage += "Non-Portable Functionality Used.";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		debugMessage += "Code Performance Issue.";
		break;

	case GL_DEBUG_TYPE_MARKER:
		debugMessage += "Marker.";
		break;

	case GL_DEBUG_TYPE_PUSH_GROUP:
		debugMessage += "Group Pushing.";
		break;

	case GL_DEBUG_TYPE_POP_GROUP:
		debugMessage += "Group Popping";
		break;

	case GL_DEBUG_TYPE_OTHER:
		debugMessage += "Other.";
		break;
	}

	debugMessage += "\nSeverity is ";
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		debugMessage += "High.";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		debugMessage += "Medium.";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		debugMessage += "Low.";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		debugMessage += "Notification.";
		break;
	}

	ENGINE_LOG_DEBUG(debugMessage);
}

void CheckOpenGlShaderCompilerErrors(const unsigned int shaderId)
{
	int compilationSuccessful;
	GL_CALL(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &compilationSuccessful);
	if(!compilationSuccessful)
	{
		int maxErrorBufferLength;
		GL_CALL(glGetShaderiv, shaderId, GL_INFO_LOG_LENGTH, &maxErrorBufferLength);
		std::vector<char> errorBuffer(maxErrorBufferLength);
		GL_CALL(glGetShaderInfoLog, shaderId, maxErrorBufferLength, NULL, errorBuffer.data());
		ENGINE_LOG_ERROR(errorBuffer.data());
	}
}

void CheckOpenGlShaderProgramLinkerErrors(const unsigned int shaderId)
{
	int compilationSuccessful;
	GL_CALL(glGetProgramiv, shaderId, GL_LINK_STATUS, &compilationSuccessful);
	if (!compilationSuccessful)
	{
		int maxErrorBufferLength;
		GL_CALL(glGetProgramiv, shaderId, GL_INFO_LOG_LENGTH, &maxErrorBufferLength);
		std::vector<char> errorBuffer(maxErrorBufferLength);
		GL_CALL(glGetProgramInfoLog, shaderId, maxErrorBufferLength, NULL, errorBuffer.data());
		ENGINE_LOG_ERROR(errorBuffer.data());
	}
}
