#pragma once

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include <vector>
#include <set>

namespace NightOwl
{
	class PhysicsEngine2D
	{
	public:
		void Update();

		void AddRigidBody2D(RigidBody2D* rigidBody2D);

		void RemoveRigidBody2D(const RigidBody2D* rigidBody2D);

	private:
		std::vector<RigidBody2D*> rigidBodies;

		std::set<RigidBody2D*> collidingBodies;
	};
}
