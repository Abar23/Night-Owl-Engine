#include "NightOwlPch.h"

#include "Animator.h"
#include "NightOwl/Animation/3D/Animation.h"
#include "NightOwl/Animation/3D/System/AnimatorSystem.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/GameObject/GameObject.h"
#include <stack>

namespace NightOwl
{
	Animator::Animator()
		: Component(ComponentType::Animator),
		  updateMode(AnimatorUpdateMode::Normal),
		  isPlaying(false),
		  currentMotion(nullptr),
		  skeleton(nullptr)
	{
		AnimatorSystemLocator::Get()->AddAnimator(this);
	}

	std::shared_ptr<Component> Animator::Clone()
	{
		std::shared_ptr<Animator> clone = std::make_shared<Animator>();

		clone->currentMotion = currentMotion;
		clone->animationCollection = animationCollection;
		clone->isPlaying = isPlaying;
		clone->currentMotion = currentMotion;

		// Since skeleton is hierarchy of child gameobjects, they will be cloned when via base gameobject clone.
		clone->skeleton = nullptr;

		return clone;
	}

	void Animator::Update()
	{
		// TODO: remove debug lines after project 1
		// Draw debug lines for bones
		std::stack<Transform*> skeletonTransforms;
		DebugSystem* debugSystem = DebugSystemLocator::Get();
		skeletonTransforms.push(skeleton);
		while (skeletonTransforms.empty() == false)
		{
			Transform* skeletonTransform = skeletonTransforms.top();
			skeletonTransforms.pop();

			Vec3F parentStartPoint = skeletonTransform->GetPosition();
			for (int skeletonTransformChildIndex = 0; skeletonTransformChildIndex < skeletonTransform->GetNumberOfChildren(); ++skeletonTransformChildIndex)
			{
				Transform* childTransform = skeletonTransform->GetChildAtIndex(skeletonTransformChildIndex);

				Vec3F childStartPosition = childTransform->GetPosition();
				debugSystem->DrawLine(parentStartPoint, childStartPosition);

				skeletonTransforms.push(childTransform);
			}
		}

		if (isPlaying == false ||
			currentMotion == nullptr ||
			animationCollection.motionsMap.empty() == true)
		{
			return;
		}

		deltaTime = Time::GetDeltaTime();
		if (updateMode == AnimatorUpdateMode::AnimatePhysics)
		{
			deltaTime = Time::GetFixedDeltaTime();
		}
		else if (updateMode == AnimatorUpdateMode::UnscaledTime)
		{
			deltaTime = Time::GetUnscaledDeltaTime();
		}

		currentMotion->Update(this);
	}

	void Animator::Play()
	{
		isPlaying = true;
	}

	void Animator::Pause()
	{
		isPlaying = false;
	}

	void Animator::Stop()
	{
		isPlaying = false;
		currentMotion->Reset();
	}

	void Animator::Reset()
	{
		currentMotion->Reset();
	}

	bool Animator::IsPlaying()
	{
		return isPlaying;
	}

	void Animator::AddMotion(Motion* motion)
	{
		if (animationCollection.motionsMap.contains(motion->GetName()))
		{
			return;
		}

		animationCollection.motionsMap.emplace(motion->GetName(), motion);
	}

	void Animator::SetCurrentMotion(const std::string& motionName)
	{
		if (animationCollection.motionsMap.contains(motionName) == false)
		{
			currentMotion = nullptr;
		}

		currentMotion = animationCollection.motionsMap[motionName];
	}

	AnimatorUpdateMode Animator::GetUpdateMode()
	{
		return updateMode;
	}

	void Animator::SetUpdateMode(AnimatorUpdateMode updateMode)
	{
		this->updateMode = updateMode;
	}

	void Animator::SetSkeleton(Transform* skeleton)
	{
		this->skeleton = skeleton;
	}

	Transform* Animator::GetSkeleton() const
	{
		return skeleton;
	}

	Motion* Animator::GetCurrentMotion() const
	{
		return currentMotion;
	}

	float Animator::GetFloat(const std::string& parameterName)
	{
		ENGINE_ASSERT(floatParameterMap.contains(parameterName), "Tried to retrieve float parameter that does not exist in the animator!");

		return floatParameterMap[parameterName];
	}

	void Animator::SetFloat(const std::string& parameterName, float value)
	{
		floatParameterMap[parameterName] = value;
	}

	void Animator::Remove()
	{
		AnimatorSystemLocator::Get()->RemoveAnimator(this);
	}
}
