#pragma once

namespace NightOwl
{
	class Animation;

	class BlendTreeNode
	{
	public:
		BlendTreeNode(Animation* animation, float threshold, float timeScale)
			: animation(animation), threshold(threshold), timeScale(timeScale)
		{}

		Animation* animation;

		float threshold;

		float timeScale;

		bool operator<(const BlendTreeNode& otherBlendTreeNode) const
		{
			return this->threshold < otherBlendTreeNode.threshold;
		}

	private:
		float speedFactor;

		float normalizedTimeScale;

		friend class BlendTree;
	};
}
