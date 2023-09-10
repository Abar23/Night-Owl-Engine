#include "NightOwlPch.h"

#include "Animation.h"

namespace NightOwl
{
	Animation::Animation()
		: duration(0),
		  ticksPerSecond(0)
	{
	}

	float Animation::GetDuration() const
	{
		return duration;
	}

	void Animation::SetDuration(float duration)
	{
		this->duration = duration;
	}

	int Animation::GetTicksPerSecond() const
	{
		return ticksPerSecond;
	}

	void Animation::SetTicksPerSecond(int ticksPerSecond)
	{
		this->ticksPerSecond = ticksPerSecond;
	}

	std::string Animation::GetName() const
	{
		return name;
	}

	void Animation::SetName(const std::string& name)
	{
		this->name = name;
	}

	BoneKeyFrames* Animation::GetBoneKeyFrames(const std::string& boneName)
	{
		if (boneKeyFramesMap.contains(boneName) == false)
		{
			return nullptr;
		}

		return &boneKeyFramesMap.at(boneName);
	}

	std::map<std::string, BoneKeyFrames>& Animation::GetBoneKeyFramesMap()
	{
		return boneKeyFramesMap;
	}
}
