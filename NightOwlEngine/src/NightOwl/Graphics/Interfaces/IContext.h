#pragma once

#include "NightOwl/Graphics/Types/BlendFunctionType.h"
#include "NightOwl/Graphics/Types/ContextCapabilityType.h"
#include "NightOwl/Graphics/Types/FaceType.h"
#include "NightOwl/Graphics/Types/DrawType.h"
#include "NightOwl/Math/Vec4.h"

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

		virtual void EnableCapability(ContextCapabilityType type, bool enable) = 0;

		virtual void CullFaceMode(FaceType type) = 0;

		virtual void ColorBlendFunction(BlendFunctionType sourceFunctionType, BlendFunctionType destinationFunctionType) = 0;

		virtual void SetViewPort(int lowerLeftX, int lowerLeftY, int width, int height) = 0;
	};
}
