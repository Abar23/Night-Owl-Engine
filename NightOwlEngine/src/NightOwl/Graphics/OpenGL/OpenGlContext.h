#pragma once

#include "NightOwl/Graphics/Interfaces/IContext.h"
#include <GLFW/glfw3.h>

namespace NightOwl
{
	class OpenGlContext : public IContext
	{
	public:
		OpenGlContext(GLFWwindow* window);

		void AttachContext() override;

		void DetachContext() override;

		void SwapBuffers() override;

		void DrawArrays(DrawType drawType, int vertexCount, int startingIndex = 0) override;

		void DrawIndexed(DrawType drawType, int numberOfIndices) override;

		void DrawIndexedBaseVertex(DrawType drawType, int numberOfIndices, int startIndex, int baseVertex) override;

		void ClearColor() override;

		void ClearBuffer() override;

		void SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight) override;

		void EnableWireFrame(bool enabled) override;

		void SetClearColor(const Vec4F& color) override;

		void AddShaderInclude(const std::string& name, const std::string& shaderIncludeSource) override;

		void DeleteShaderInclude(const std::string& name) override;

	private:
		GLFWwindow* window;

		Vec4F clearColor;

		const Vec4F DEFAULT_CLEAR_COLOR = Vec4F(0.5f, 0.4f, 0.3f, 1.0f);
	};
}

