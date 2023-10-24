#include "NightOwlPch.h"
#include "Motion.h"

namespace NightOwl
{
	Motion::Motion(MotionType type)
		: elapsedTime(0),
		  type(type)
	{ }

	void Motion::Reset()
	{
		elapsedTime = 0.0f;
	}

	MotionType Motion::GetMotionType() const
	{
		return type;
	}

	std::string Motion::GetName() const
	{
		return name;
	}

	void Motion::SetName(const std::string& name)
	{
		this->name = name;
	}

	float Motion::GetElapsedTime() const
	{
		return elapsedTime;
	}

	void Motion::SetElapsedTime(float elapsedTime)
	{
		this->elapsedTime = elapsedTime;
	}
}
