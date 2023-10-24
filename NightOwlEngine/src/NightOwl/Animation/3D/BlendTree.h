#pragma once

#include "Motion.h"
#include "Structures/BlendTreeNode.h"
#include "Types/BlendTreeType.h"
#include <string>
#include <vector>

namespace NightOwl
{
	class BlendTree : public Motion
	{
	public:
		BlendTree();

		void Update(Animator* animator) override;

		void AddBlendTreeNode(const BlendTreeNode& blendTreeNode);

		void RemoveBlendTreeNodeAtIndex(int index);

		const std::vector<BlendTreeNode>& GetBlendTreeNodes();

		const std::string& GetBlendParameterXName();

		void SetBlendParameterXName(const std::string& parameterName);

	private:
		float maxThreshold;

		float minThreshold;

		float maxDuration;

		float minDuration;

		float maxDurationInSeconds;

		float maxToMinDurationRatio;

		BlendTreeType blendType;

		bool isDirty;

		std::string blendParameterX;

		std::vector<BlendTreeNode> nodes;

		// TODO: Min, Max, and automatic thresholds

		void UpdateSimple1D(Animator* animator, float deltaTime);

		int GetLowerBoundBlendTreeNodeIndexForBlendValue(float blendValue) const;

		void UpdateElapsedTime(float deltaTime);

		void UpdateBlendTreeIfDirty();
	};
}
