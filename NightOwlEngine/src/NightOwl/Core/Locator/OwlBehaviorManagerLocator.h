#pragma once

#include "NightOwl/Behavior/OwlBehaviorManager.h"

namespace NightOwl
{
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
