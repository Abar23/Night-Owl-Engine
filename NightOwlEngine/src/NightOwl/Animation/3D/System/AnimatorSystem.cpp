#include "NightOwlPch.h"

#include "NightOwl/GameObject/GameObject.h"
#include "AnimatorSystem.h"

namespace NightOwl
{
	void AnimatorSystem::Update()
	{
		for (const auto& animator : animators)
		{
			if (animator->GetGameObject().IsActive() &&
				animator->GetUpdateMode() != AnimatorUpdateMode::AnimatePhysics)
			{
				animator->Update();
			}
		}
	}

	void AnimatorSystem::FixedUpdate()
	{
		for (const auto& animator : animators)
		{
			if (animator->GetGameObject().IsActive() &&
				animator->GetUpdateMode() == AnimatorUpdateMode::AnimatePhysics)
			{
				animator->Update();
			}
		}
	}

	void AnimatorSystem::AddAnimator(Animator* animator)
	{
		animators.push_back(animator);
	}

	void AnimatorSystem::RemoveAnimator(const Animator* animator)
	{
		int animatorIndex = 0;
		for (const auto& animatorToFind : animators)
		{
			if (animatorToFind == animator)
			{
				break;
			}
			animatorIndex++;
		}

		if (animatorIndex != animators.size())
		{
			animators.erase(animators.begin() + animatorIndex);
		}
	}
}
