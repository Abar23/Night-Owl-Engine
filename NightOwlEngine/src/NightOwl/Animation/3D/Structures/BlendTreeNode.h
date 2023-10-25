#pragma once

namespace NightOwl
{
	class Animation;

	class BlendTreeNode
	{
	public:
		Animation* animation;

		float threshold;

		float timeScale;

		bool operator<(const BlendTreeNode& otherBlendTreeNode) const
		{
			return this->threshold < otherBlendTreeNode.threshold;
		}
	};
}
