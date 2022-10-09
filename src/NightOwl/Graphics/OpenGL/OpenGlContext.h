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

	private:
		GLFWwindow* window;
	};
}

