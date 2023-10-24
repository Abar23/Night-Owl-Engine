#include "NightOwlPch.h"

#include "Animation.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include <stack>

namespace NightOwl
{
	Animation::Animation()
		: Motion(MotionType::Animation),
		  ticksPerSecond(0),
		  duration(0)
	{ }

	void Animation::Update(Animator* animator)
	{
		elapsedTime += Time::GetDeltaTime() * ticksPerSecond;

		if (elapsedTime > duration)
		{
			elapsedTime = 0.0f;
		}

		std::stack<Transform*> skeletonTransforms;
		skeletonTransforms.push(animator->skeleton);

		while (skeletonTransforms.empty() == false)
		{
			Transform* skeletonTransform = skeletonTransforms.top();
			skeletonTransforms.pop();

			for (int skeletonTransformChildIndex = 0; skeletonTransformChildIndex < skeletonTransform->GetNumberOfChildren(); ++skeletonTransformChildIndex)
			{
				skeletonTransforms.push(skeletonTransform->GetChildAtIndex(skeletonTransformChildIndex));
			}

			KeyFrames* keyFrames = GetKeyFrames(skeletonTransform->GetGameObject().GetName());
			if (keyFrames != nullptr)
			{
				keyFrames->Update(elapsedTime);

				skeletonTransform->SetLocalPosition(keyFrames->GetFinalPosition());
				skeletonTransform->SetLocalRotation(keyFrames->GetFinalRotation());
				skeletonTransform->SetLocalScale(keyFrames->GetFinalScale());
			}
		}
	}

	float Animation::GetDuration() const
	{
		return duration;
	}

	void Animation::SetDuration(float duration)
	{
		this->duration = duration;
	}

	float Animation::GetTicksPerSecond() const
	{
		return ticksPerSecond;
	}

	void Animation::SetTicksPerSecond(float ticksPerSecond)
	{
		this->ticksPerSecond = ticksPerSecond;
	}

	KeyFrames* Animation::GetKeyFrames(const std::string& gameObjectName)
	{
		if (keyFramesMap.contains(gameObjectName) == false)
		{
			return nullptr;
		}

		return &keyFramesMap.at(gameObjectName);
	}

	std::map<std::string, KeyFrames>& Animation::GetKeyFrames()
	{
		return keyFramesMap;
	}
}
