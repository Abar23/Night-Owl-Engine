#pragma once

#include "NightOwl/Graphics/Types/DrawType.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl::Graphics
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AttachContext() = 0;

		virtual void DetachContext() = 0;

		virtual void SwapBuffers() = 0;

		virtual void DrawIndexed(DrawType drawType, int numberOfIndices) = 0;

		virtual void ClearColor(Math::Vec4F color) = 0;

		virtual void ClearBuffer() = 0;

		virtual void SetViewport(int cornerX, int cornerY, int viewWidth, int viewHeight) = 0;

		virtual void EnableWireframe(bool enabled) = 0;
	};
}
