#include "NightOwlPch.h"

#include "Animator.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include <stack>

#include "MeshRenderer.h"
#include "NightOwl/Graphics/Materials/IMaterial.h"

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

		if (updateMode == AnimatorUpdateMode::AnimatePhysics)
		{
			elapsedTime += Time::GetFixedDeltaTime() * currentAnimation->GetTicksPerSecond();
		}
		else if (updateMode == AnimatorUpdateMode::UnscaledTime)
		{
			elapsedTime += Time::GetUnscaledDeltaTime() * currentAnimation->GetTicksPerSecond();
		}
		else
		{
			// TODO: figure out rotation shit tomorrow
			elapsedTime += Time::GetDeltaTime() * currentAnimation->GetTicksPerSecond() / 10.0f;
		}

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
		std::vector<Mat4F> finalBoneOffsetMatrices(100);

		std::stack<Transform*> skeletonTransforms;
		skeletonTransforms.push(skeleton);
		while (skeletonTransforms.empty() == false)
		{
			Transform* skeletonTransform = skeletonTransforms.top();
			skeletonTransforms.pop();

			BoneKeyFrames* boneKeyFrames = currentAnimation->GetBoneKeyFrames(skeletonTransform->GetGameObject().GetName());
			if (boneKeyFrames != nullptr)
			{
				boneKeyFrames->Update(elapsedTime);
				
				skeletonTransform->SetLocalPosition(boneKeyFrames->GetFinalPosition());
				skeletonTransform->SetLocalScale(boneKeyFrames->GetFinalScale());
				skeletonTransform->SetLocalRotation(boneKeyFrames->GetFinalRotation());

				const auto& boneName = skeletonTransform->GetGameObject().GetName();
				BoneInfo boneInfo = bonInfoMap.at(boneName);
				const Mat4F finalBoneOffsetMatrix = skeletonTransform->GetWorldMatrix() * boneInfo.offsetMatrix;
				finalBoneOffsetMatrices[boneInfo.id] = finalBoneOffsetMatrix;
			}

			for (int skeletonTransformChildIndex = 0; skeletonTransformChildIndex < skeletonTransform->GetNumberOfChildren(); ++skeletonTransformChildIndex)
			{
				skeletonTransforms.push(skeletonTransform->GetChildAtIndex(skeletonTransformChildIndex));
			}
		}

		renderer->GetMaterial()->SetFinalBoneMatrices(finalBoneOffsetMatrices);
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
