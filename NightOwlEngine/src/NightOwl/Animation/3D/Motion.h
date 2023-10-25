#pragma once

#include "Types/MotionType.h"
#include <string>

namespace NightOwl
{
	class Animator;

	class Motion
	{
	public:
		Motion(MotionType type);

		virtual void Update(Animator* animator) = 0;

		void Reset();

		MotionType GetMotionType() const;

		std::string GetName() const;

		void SetName(const std::string& name);

		float GetElapsedTime() const;

		void SetElapsedTime(float elapsedTime);

	protected:
		float elapsedTime;

		MotionType type;

		std::string name;
	};
}
