#pragma once

#include "NightOwl/Component/Concrete/Animator.h"

namespace NightOwl
{
	class AnimatorSystem
	{
	public:
		void Update();

		void FixedUpdate();

		void AddAnimator(Animator* animator);

		void RemoveAnimator(const Animator* animator);

	private:
		std::vector<Animator*> animators;
	};
}
