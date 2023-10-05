#pragma once

namespace NightOwl
{
	class AnimatorSystem;

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


