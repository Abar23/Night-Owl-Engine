#pragma once

namespace NightOwl
{
	class OwlBehaviorManager;

	class OwlBehaviorManagerLocator
	{
	public:
		static OwlBehaviorManager* GetOwlBehaviorManager()
		{
			return owlBehaviorManagerInstance;
		}

		static void Provide(OwlBehaviorManager* owlBehaviorManager)
		{
			owlBehaviorManagerInstance = owlBehaviorManager;
		}

	private:
		inline static OwlBehaviorManager* owlBehaviorManagerInstance{ nullptr };
	};
}
