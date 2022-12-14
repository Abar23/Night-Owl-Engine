#include "OpenGlContext.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "glad/glad.h"

namespace NightOwl::Graphics
{
	OpenGlContext::OpenGlContext(GLFWwindow* window)
		:	window(window)
	{
		glfwMakeContextCurrent(window);
		int gladLoadedSucessfully = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_ASSERT(gladLoadedSucessfully, "Failed to load glad!");
		ENGINE_ASSERT(GLVersion.major == 4 && GLVersion.minor == 6, "OpenGL version 4.6 is required for NightOwl!");
		
		const unsigned char* openGlInfo = GL_CALL(glGetString, GL_VENDOR);
		ENGINE_LOG_INFO("OpenGL vendor: {0}", reinterpret_cast<const char *>(openGlInfo));

		openGlInfo = GL_CALL(glGetString, GL_RENDERER);
		ENGINE_LOG_INFO("OpenGL renderer: {0}", reinterpret_cast<const char*>(openGlInfo));

		openGlInfo = GL_CALL(glGetString, GL_VERSION);
		ENGINE_LOG_INFO("OpenGL vendor: {0}", reinterpret_cast<const char*>(openGlInfo));

		#ifdef DEBUG
		GL_CALL(glEnable, GL_DEBUG_OUTPUT);
		GL_CALL(glEnable, GL_DEBUG_OUTPUT_SYNCHRONOUS);
		GL_CALL(glDebugMessageCallback, Utility::GlDebugOutput, nullptr);

		GL_CALL(glDebugMessageControl, GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_FALSE);
		#endif

		GL_CALL(glEnable, GL_BLEND);
		GL_CALL(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GL_CALL(glEnable, GL_DEPTH_TEST);
		GL_CALL(glEnable, GL_LINE_SMOOTH);
	}

	void OpenGlContext::AttachContext()
	{
		glfwMakeContextCurrent(window);
	}

	void OpenGlContext::DetachContext()
	{
		glfwMakeContextCurrent(nullptr);
	}

	void OpenGlContext::SwapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void OpenGlContext::DrawIndexed(DrawType drawType, int numberOfIndices)
	{
		int openGlDrawType = DrawTypeToOpenGlDrawType(drawType);
		GL_CALL(glDrawElements, openGlDrawType, numberOfIndices, GL_UNSIGNED_INT, 0);
	}

	void OpenGlContext::ClearColor(Math::Vec4F color)
	{
		GL_CALL(glClearColor, color.x, color.y, color.z, color.w);
	}

	void OpenGlContext::ClearBuffer()
	{
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlContext::SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight)
	{
		GL_CALL(glViewport, cornerX, cornerY, viewWidth, viewHeight);
	}

	void OpenGlContext::EnableWireframe(bool enabled)
	{
		const int mode = enabled ? GL_LINE : GL_FILL;

		GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, mode);
	}
}
