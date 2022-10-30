#pragma once

#include "NightOwl/Behavior/OwlBehaviorManager.h"

namespace NightOwl::Core
{
	class OwlBehaviorManagerLocator
	{
	public:
		static Behavior::OwlBehaviorManager* GetOwlBehaviorManager()
		{
			return owlBehaviorManagerInstance;
		}

		static void Provide(Behavior::OwlBehaviorManager* owlBehaviorManager)
		{
			owlBehaviorManagerInstance = owlBehaviorManager;
		}

	private:
		inline static Behavior::OwlBehaviorManager* owlBehaviorManagerInstance = nullptr;
	};
}
