#pragma once

namespace NightOwl
{
	class DebugSystem;

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
