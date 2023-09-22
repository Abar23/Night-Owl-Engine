#include "NightOwlPch.h"

#include "Animator.h"
#include "MeshRenderer.h"
#include "NightOwl/Graphics/Materials/Material.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include <stack>

namespace NightOwl
{
	Animator::Animator()
		: Component(ComponentType::Animator),
		  updateMode(AnimatorUpdateMode::Normal),
		  elapsedTime(0.0f),
		  isPlaying(false),
		  currentAnimation(nullptr),
		  skeleton(nullptr)
	{
		AnimatorSystemLocator::GetAnimatorSystem()->AddAnimator(this);
	}

	std::shared_ptr<Component> Animator::Clone()
	{
		std::shared_ptr<Animator> clone = std::make_shared<Animator>();

		clone->currentAnimation = currentAnimation;
		clone->animationCollection = animationCollection;
		clone->elapsedTime = elapsedTime;
		clone->isPlaying = isPlaying;
		// TODO: skeleton will need to be cloned otherwise it will effect the same transforms of original
		clone->skeleton = skeleton;
		clone->currentAnimation = currentAnimation;

		return clone;
	}

	void Animator::Update()
	{
		if (isPlaying == false ||
			currentAnimation == nullptr ||
			animationCollection.animationsMap.empty() == true)
		{
			return;
		}

		float deltaTime = Time::GetDeltaTime();
		if (updateMode == AnimatorUpdateMode::AnimatePhysics)
		{
			deltaTime = Time::GetFixedDeltaTime();
		}
		else if (updateMode == AnimatorUpdateMode::UnscaledTime)
		{
			deltaTime = Time::GetUnscaledDeltaTime();
		}

		elapsedTime += deltaTime * currentAnimation->GetTicksPerSecond();

		if (elapsedTime > currentAnimation->GetDuration())
		{
			elapsedTime = 0.0f;
		}

		auto* renderer = gameObject->GetComponent<MeshRenderer>();
		if (renderer == nullptr)
		{
			return;
		}
		
		auto& bonInfoMap = renderer->GetMesh()->GetBoneInfoMap();

		// TODO: Do this in a buffer
		std::vector<Mat4F>& finalBoneOffsetMatrices = renderer->GetFinalBoneMatrices();
		if (finalBoneOffsetMatrices.empty() == true)
		{
			finalBoneOffsetMatrices.resize(100);
		}

		std::stack<Transform*> skeletonTransforms;
		skeletonTransforms.push(skeleton);
		while (skeletonTransforms.empty() == false)
		{
			Transform* skeletonTransform = skeletonTransforms.top();
			skeletonTransforms.pop();

			for (int skeletonTransformChildIndex = 0; skeletonTransformChildIndex < skeletonTransform->GetNumberOfChildren(); ++skeletonTransformChildIndex)
			{
				skeletonTransforms.push(skeletonTransform->GetChildAtIndex(skeletonTransformChildIndex));
			}

			BoneKeyFrames* boneKeyFrames = currentAnimation->GetBoneKeyFrames(skeletonTransform->GetGameObject().GetName());
			if (boneKeyFrames != nullptr)
			{
				boneKeyFrames->Update(elapsedTime);

				skeletonTransform->SetLocalPosition(boneKeyFrames->GetFinalPosition());
				skeletonTransform->SetLocalRotation(boneKeyFrames->GetFinalRotation());
				skeletonTransform->SetLocalScale(boneKeyFrames->GetFinalScale());

				const auto& boneName = skeletonTransform->GetGameObject().GetName();
				BoneInfo boneInfo = bonInfoMap.at(boneName);
				const Mat4F finalBoneOffsetMatrix = skeletonTransform->GetWorldMatrix() * boneInfo.offsetMatrix;
				finalBoneOffsetMatrices[boneInfo.id] = finalBoneOffsetMatrix;
			}
		}
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
		elapsedTime = 0.0f;
	}

	void Animator::Reset()
	{
		elapsedTime = 0.0f;
	}

	bool Animator::IsPlaying()
	{
		return isPlaying;
	}

	void Animator::AddAnimation(Animation* animation)
	{
		if (animationCollection.animationsMap.contains(animation->GetName()))
		{
			return;
		}

		animationCollection.animationsMap.emplace(animation->GetName(), animation);
	}

	void Animator::SetCurrentAnimation(const std::string& animationName)
	{
		if (animationCollection.animationsMap.contains(animationName) == false)
		{
			currentAnimation = nullptr;
		}

		currentAnimation = animationCollection.animationsMap[animationName];
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

	void Animator::Remove()
	{
		AnimatorSystemLocator::GetAnimatorSystem()->RemoveAnimator(this);
	}
}
