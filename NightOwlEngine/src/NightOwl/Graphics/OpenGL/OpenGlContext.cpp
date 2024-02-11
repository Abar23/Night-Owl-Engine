#include <NightOwlPch.h>

#include "OpenGlContext.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace NightOwl
{
	OpenGlContext::OpenGlContext(GLFWwindow* window)
		:	window(window),
			clearColor(DEFAULT_CLEAR_COLOR)
	{
		glfwMakeContextCurrent(window);
		const int gladLoadedSuccessfully = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
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

		EnableCapability(ContextCapabilityType::ColorBlend, true);
		EnableCapability(ContextCapabilityType::DepthTest, true);
		EnableCapability(ContextCapabilityType::LineSmooth, true);
		EnableCapability(ContextCapabilityType::VertexProgramPointSize, true);
		EnableCapability(ContextCapabilityType::CullFace, true);

		CullFaceMode(FaceType::Back);

		ColorBlendFunction(BlendFunctionType::SourceAlpha, BlendFunctionType::OneMinusSourceAlpha);
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

	void OpenGlContext::DrawArrays(DrawType drawType, int vertexCount, int startingIndex /* = 0 */)
	{
		const int openGlDrawType = DrawTypeToOpenGlDrawType(drawType);
		GL_CALL(glDrawArrays, openGlDrawType, startingIndex, vertexCount);
	}

	void OpenGlContext::DrawIndexed(DrawType drawType, int numberOfIndices)
	{
		const int openGlDrawType = DrawTypeToOpenGlDrawType(drawType);
		GL_CALL(glDrawElements, openGlDrawType, numberOfIndices, GL_UNSIGNED_INT, 0);
	}

	void OpenGlContext::DrawIndexedBaseVertex(DrawType drawType, int numberOfIndices, int startIndex, int baseVertex)
	{
		const int openGlDrawType = DrawTypeToOpenGlDrawType(drawType);
		GL_CALL(glDrawElementsBaseVertex, openGlDrawType, numberOfIndices, GL_UNSIGNED_INT, reinterpret_cast<void*>(startIndex * sizeof(unsigned int)), baseVertex);
	}

	void OpenGlContext::ClearColor()
	{
		GL_CALL(glClearColor, clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	}

	void OpenGlContext::ClearBuffer()
	{
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlContext::SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight)
	{
		GL_CALL(glViewport, cornerX, cornerY, viewWidth, viewHeight);
	}

	void OpenGlContext::EnableWireFrame(bool enabled)
	{
		const int mode = enabled ? GL_LINE : GL_FILL;

		GL_CALL(glPolygonMode, GL_FRONT_AND_BACK, mode);
	}

	void OpenGlContext::SetClearColor(const Vec4F& color)
	{
		clearColor = color;
	}

	void OpenGlContext::EnableCapability(ContextCapabilityType type, bool enable)
	{
		const int openGlCapabilityType = ContextCapabilityTypeToOpenGlType(type);
		if (openGlCapabilityType == -1)
		{
			return;
		}

		enable ? GL_CALL(glEnable, openGlCapabilityType) : GL_CALL(glDisable, openGlCapabilityType);
	}

	void OpenGlContext::CullFaceMode(FaceType type)
	{
		GL_CALL(glCullFace, CullFaceTypeToOpenGlType(type));
	}

	void OpenGlContext::ColorBlendFunction(BlendFunctionType sourceFunctionType, BlendFunctionType destinationFunctionType)
	{
		const int sourceOpenGlBlendFunctionType = BlendFunctionTypeToOpenGlType(sourceFunctionType);
		const int destinationOGlBlendFunctionType = BlendFunctionTypeToOpenGlType(destinationFunctionType);
		if (sourceOpenGlBlendFunctionType == -1 ||
			destinationOGlBlendFunctionType == -1)
		{
			return;
		}

		GL_CALL(glBlendFunc, sourceOpenGlBlendFunctionType, destinationOGlBlendFunctionType);
	}
}
