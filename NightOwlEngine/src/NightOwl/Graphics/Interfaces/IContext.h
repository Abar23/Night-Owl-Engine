#pragma once

#include "NightOwl/Graphics/Types/DrawType.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AttachContext() = 0;

		virtual void DetachContext() = 0;

		virtual void SwapBuffers() = 0;

		virtual void DrawArrays(DrawType drawType, int vertexCount, int startingIndex = 0) = 0;

		virtual void DrawIndexed(DrawType drawType, int numberOfIndices) = 0;

		virtual void DrawIndexedBaseVertex(DrawType drawType, int numberOfIndices, int startIndex, int baseVertex) = 0;

		virtual void ClearColor() = 0;

		virtual void ClearBuffer() = 0;

		virtual void SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight) = 0;

		virtual void EnableWireFrame(bool enabled) = 0;

		virtual void SetClearColor(const Vec4F& color) = 0;

		virtual void AddShaderInclude(const std::string& name, const std::string& shaderIncludeSource) = 0;

		virtual void DeleteShaderInclude(const std::string& name) = 0;
	};
}
