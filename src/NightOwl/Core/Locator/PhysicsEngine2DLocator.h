#pragma once

#include "NightOwl/Physics/2D/PhysicsEngine2D.h"

namespace NightOwl::Core
{
	class PhysicsEngine2DLocator
	{
	public:
		static Physics::PhysicsEngine2D* GetPhysicsEngine2D()
		{
			return physicsEngine2DInstance;
		}

		static void Provide(Physics::PhysicsEngine2D* physicsEngine2D)
		{
			physicsEngine2DInstance = physicsEngine2D;
		}

	private:
		inline static Physics::PhysicsEngine2D* physicsEngine2DInstance{ nullptr };
	};
}
