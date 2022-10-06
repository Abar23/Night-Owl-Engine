#pragma once

namespace NightOwl::Graphics
{
	class IContext
	{
	public:
		virtual ~IContext() = default;

		virtual void AttachContext() = 0;
		virtual void DetachContext() = 0;

		virtual void SwapBuffers() = 0;
	};
}
