#pragma once

#include "NightOwl/Animation/3D/System/AnimatorSystem.h"

namespace NightOwl
{
	class AnimatorSystemLocator
	{
	public:
		static AnimatorSystem* GetAnimatorSystem()
		{
			return animatorSystem;
		}

		static void Provide(AnimatorSystem* AnimatorSystemInstance)
		{
			animatorSystem = AnimatorSystemInstance;
		}

	private:
		inline static AnimatorSystem* animatorSystem{ nullptr };
	};
}


