#pragma once

#include <vector>

namespace NightOwl
{
	class Animator;

	class ChainIK;

	class AnimatorSystem
	{
	public:
		void Update();

		void FixedUpdate();

		void AddChainIk(ChainIK* chainIk);

		void RemoveChainIk(const ChainIK* chainIkToFind);

		void AddAnimator(Animator* animator);

		void RemoveAnimator(const Animator* animator);

	private:
		std::vector<Animator*> animators;

		std::vector<ChainIK*> chainIkComponents;
	};
}
