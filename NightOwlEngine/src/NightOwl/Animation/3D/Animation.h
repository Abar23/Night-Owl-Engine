#pragma once

#include "Motion.h"
#include "Structures/KeyFrames.h"
#include <map>
#include <string>

#include "NightOwl/Component/Concrete/Transform.h"

namespace NightOwl
{
	class Animation : public Motion
	{
	public:
		Animation();

		void Update(Animator* animator) override;

		float GetDuration() const;

		void SetDuration(float duration);

		float GetTicksPerSecond() const;

		void SetTicksPerSecond(float ticksPerSecond);

		KeyFrames* GetKeyFrames(const std::string& gameObjectName);

		std::map<std::string, KeyFrames>& GetKeyFrames();

	private:
		float ticksPerSecond;

		float duration;

		std::map<std::string, KeyFrames> keyFramesMap;
	};
}
