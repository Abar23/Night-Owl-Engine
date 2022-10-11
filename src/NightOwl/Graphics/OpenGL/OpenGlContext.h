#pragma once

#include "NightOwl/Graphics/Interfaces/IContext.h"
#include "GLFW/glfw3.h"

namespace NightOwl::Graphics
{
	class OpenGlContext : public IContext
	{
	public:
		OpenGlContext(GLFWwindow* window);

		void AttachContext() override;

		void DetachContext() override;

		void SwapBuffers() override;

		void DrawIndexed(DrawType drawType, int numberOfIndices) override;

		void ClearColor(Math::Vec4F color) override;

		void ClearBuffer() override;

		void SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight) override;

		void EnableWireframe(bool enabled) override;

	private:
		GLFWwindow* window;
	};
}

