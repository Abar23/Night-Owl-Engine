#pragma once

#include "NightOwl/Graphics/Debugging/DebugSystem.h"

namespace NightOwl
{
	class DebugSystemLocator
	{
	public:
		static DebugSystem* GetDebugSystem()
		{
			return debugSystemInstance;
		}

		static void Provide(DebugSystem* debugSystem)
		{
			debugSystemInstance = debugSystem;
		}

	private:
		inline static DebugSystem* debugSystemInstance{ nullptr };
	};
}
