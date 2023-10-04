#pragma once

#include "Structures/BoneKeyFrames.h"
#include <map>
#include <string>


namespace NightOwl
{
	class Animation
	{
	public:
		Animation();

		float GetDuration() const;

		void SetDuration(float duration);

		int GetTicksPerSecond() const;

		void SetTicksPerSecond(int ticksPerSecond);

		std::string GetName() const;

		void SetName(const std::string& name);

		BoneKeyFrames* GetBoneKeyFramesMap(const std::string& boneName);

		std::map<std::string, BoneKeyFrames>& GetBoneKeyFramesMap();

	private:
		float duration;

		int ticksPerSecond;

		std::string name;

		std::map<std::string, BoneKeyFrames> boneKeyFramesMap;
	};
}
