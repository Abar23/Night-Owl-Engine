#pragma once

#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include <vector>

namespace NightOwl::Physics
{
	class PhysicsEngine2D
	{
	public:
		void Update();

		void AddRigidBody2D(Component::RigidBody2D* rigidBody2D);

		void RemoveRigidBody2D(Component::RigidBody2D* rigidBody2D);

	private:
		std::vector <Component::RigidBody2D*> rigidBodies;
	};
}