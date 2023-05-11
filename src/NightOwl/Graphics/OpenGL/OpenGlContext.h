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

		void ClearColor() override;

		void ClearBuffer() override;

		void SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight) override;

		void EnableWireFrame(bool enabled) override;

		void SetClearColor(const Math::Vec4F& color) override;

	private:
		GLFWwindow* window;

		Math::Vec4F clearColor;

		const Math::Vec4F DEFAULT_CLEAR_COLOR = Math::Vec4F(0.5f, 0.4f, 0.3f, 1.0f);
	};
}

