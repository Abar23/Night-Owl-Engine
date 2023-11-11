#include "NightOwlPch.h"

#include "AnimatorSystem.h"
#include "NightOwl/Animation/Types/AnimatorUpdateMode.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/ChainIK.h"
#include "NightOwl/GameObject/GameObject.h"

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

		for (const auto& chainIkComponent : chainIkComponents)
		{
			chainIkComponent->Update();
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

	void AnimatorSystem::AddChainIk(ChainIK* chainIk)
	{
		chainIkComponents.push_back(chainIk);
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
