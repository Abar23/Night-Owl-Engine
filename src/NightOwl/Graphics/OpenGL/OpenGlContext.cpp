#include "OpenGlContext.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "glad/glad.h"

namespace NightOwl::Graphics
{
	OpenGlContext::OpenGlContext(GLFWwindow* window)
		: window(window)
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
}
