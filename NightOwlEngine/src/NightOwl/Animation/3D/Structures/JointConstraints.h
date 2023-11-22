#pragma once

#include "NightOwl/Animation/3D/Types/JointConstraintType.h"

namespace NightOwl
{
	class minMaxJointAngles
	{
	public:
		float min;

		float max;
	};

	class BallAndSocketConstraint
	{
	public:
		minMaxJointAngles xAxisJointAngles;

		minMaxJointAngles yAxisJointAngles;

		minMaxJointAngles zAxisJointAngles;

		JointConstraintType type = JointConstraintType::BallAndSocket;
	};
}
