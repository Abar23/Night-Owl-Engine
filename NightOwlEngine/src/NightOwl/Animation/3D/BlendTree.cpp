#include "NightOwlPch.h"

#include "BlendTree.h"

#include <stack>

#include "Animation.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Core/Utitlity/Assert.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	BlendTree::BlendTree()
		: Motion(MotionType::BlendTree),
		  maxThreshold(0),
		  minThreshold(0),
		  maxDuration(0),
		  minDuration(0),
		  maxDurationInSeconds(0),
		  maxToMinDurationRatio(0),
		  blendType(BlendTreeType::Simple1D),
		  isDirty(false)
	{ }

	void BlendTree::Update(Animator* animator)
	{
		if (nodes.empty())
		{
			return;
		}

		UpdateBlendTreeIfDirty();

		switch (blendType)
		{
			case BlendTreeType::Simple1D:
			default:
				UpdateSimple1D(animator, animator->deltaTime);
				break;
		}
	}

	void BlendTree::AddBlendTreeNode(const BlendTreeNode& blendTreeNode)
	{
		nodes.push_back(blendTreeNode);

		isDirty = true;
	}

	void BlendTree::RemoveBlendTreeNodeAtIndex(int index)
	{
		ENGINE_ASSERT(index > 0 && index < nodes.size(), "Invalid index when removing blend tree node!");

		std::swap(nodes[index], nodes.back());
		nodes.pop_back();

		isDirty = true;
	}

	const std::vector<BlendTreeNode>& BlendTree::GetBlendTreeNodes()
	{
		return nodes;
	}

	const std::string& BlendTree::GetBlendParameterXName()
	{
		return blendParameterX;
	}

	void BlendTree::SetBlendParameterXName(const std::string& parameterName)
	{
		blendParameterX = parameterName;
	}

	void BlendTree::UpdateSimple1D(Animator* animator, float deltaTime)
	{
		if (blendParameterX.empty() ||
			animator->floatParameterMap.contains(blendParameterX) == false)
		{
			return;
		}

		const float blendParameterValue = std::clamp(animator->floatParameterMap[blendParameterX], minThreshold, maxThreshold);
		const int lowerBoundBlendTreeNodeIndex = GetLowerBoundBlendTreeNodeIndexForBlendValue(blendParameterValue);

		// blend between keyframes
		const BlendTreeNode& firstNode = nodes.at(lowerBoundBlendTreeNodeIndex);

		// just update the single animation held in the blend tree
		if (nodes.size() == 1)
		{
			UpdateElapsedTime(deltaTime);
			firstNode.animation->Update(animator);
			return;
		}

		const BlendTreeNode& secondNode = nodes.at(lowerBoundBlendTreeNodeIndex + 1);

		Animation* firstAnimation = firstNode.animation;
		Animation* secondAnimation = secondNode.animation;

		float blendFactor = (blendParameterValue - firstNode.threshold) / (secondNode.threshold - firstNode.threshold);

		// max to min ratio gives the proportion between the fastest and shortest animations in the blend tree
		float speedFactor = ((blendParameterValue - minThreshold) / (maxThreshold - minThreshold)) * maxToMinDurationRatio; 

		// Speed multipliers to correctly transition from one animation to another
		float firstAnimationSpeed = firstAnimation->GetDuration() / maxDuration;
		float secondAnimationSpeed = secondAnimation->GetDuration() / maxDuration;

		UpdateElapsedTime(deltaTime * speedFactor);

		float firstAnimationElapsedTime = elapsedTime * firstAnimationSpeed * firstAnimation->GetTicksPerSecond() * firstNode.timeScale;
		float secondAnimationElapsedTime = elapsedTime * secondAnimationSpeed * secondAnimation->GetTicksPerSecond() * secondNode.timeScale;

		firstAnimationElapsedTime = std::fmod(firstAnimationElapsedTime, firstAnimation->GetDuration());
		secondAnimationElapsedTime = std::fmod(secondAnimationElapsedTime, secondAnimation->GetDuration());

		// blend key frames
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

			KeyFrames* firsKeyFrames = firstAnimation->GetKeyFrames(skeletonTransform->GetGameObject().GetName());
			KeyFrames* secondKeyFrames = secondAnimation->GetKeyFrames(skeletonTransform->GetGameObject().GetName());
			if (firsKeyFrames != nullptr && secondKeyFrames != nullptr)
			{
				firsKeyFrames->Update(firstAnimationElapsedTime);
				secondKeyFrames->Update(secondAnimationElapsedTime);

				Vec3F position = Vec3F::Lerp(firsKeyFrames->GetFinalPosition(), secondKeyFrames->GetFinalPosition(), blendFactor);
				QuatF rotation = QuatF::Slerp(firsKeyFrames->GetFinalRotation(), secondKeyFrames->GetFinalRotation(), blendFactor);
				Vec3F scale = Vec3F::Elerp(firsKeyFrames->GetFinalScale(), secondKeyFrames->GetFinalScale(), blendFactor);

				skeletonTransform->SetLocalPosition(position);
				skeletonTransform->SetLocalRotation(rotation);
				skeletonTransform->SetLocalScale(scale);
			}
		}
	}

	int BlendTree::GetLowerBoundBlendTreeNodeIndexForBlendValue(float blendValue) const
	{
		// Only calculate lower bound index when there are more than 2 animations to blend between
		if (nodes.size() == 2)
		{
			return 0;
		}

		int blendTreeIndex = 0;
		while (blendTreeIndex < nodes.size() - 2)
		{
			if (blendValue >= nodes[blendTreeIndex].threshold &&
				blendValue < nodes[blendTreeIndex + 1].threshold)
			{
				break;
			}

			++blendTreeIndex;
		}

		return blendTreeIndex;
	}

	void BlendTree::UpdateElapsedTime(float deltaTime)
	{
		elapsedTime += deltaTime;
		elapsedTime = std::fmod(elapsedTime, maxDurationInSeconds);
	}

	void BlendTree::UpdateBlendTreeIfDirty()
	{
		if (isDirty == false)
		{
			return;
		}

		isDirty = false;

		// with one node, there is no need to calculate the data below since we will only update one animation ever
		if (nodes.size() == 1)
		{
			return;
		}

		std::sort(nodes.begin(), nodes.end());

		minThreshold = nodes.front().threshold;
		maxThreshold = nodes.back().threshold;

		maxDuration = std::numeric_limits<float>::min();
		minDuration = std::numeric_limits<float>::max();
		for (const auto & blendTreeNode : nodes)
		{
			const Animation* animation = blendTreeNode.animation;

			if (animation->GetDuration() < minDuration)
			{
				minDuration = animation->GetDuration();
			}

			if (animation->GetDuration() > maxDuration)
			{
				maxDuration = animation->GetDuration();
				maxDurationInSeconds = maxDuration / animation->GetTicksPerSecond();
			}
		}

		maxToMinDurationRatio = maxDuration / minDuration;
	}
}
