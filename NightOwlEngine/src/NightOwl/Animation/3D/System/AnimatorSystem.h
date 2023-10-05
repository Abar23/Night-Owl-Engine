#pragma once

#include <vector>

namespace NightOwl
{
	class Animator;

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
