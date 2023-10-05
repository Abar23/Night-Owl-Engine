#pragma once

namespace NightOwl
{
	class PhysicsEngine2D;

	class PhysicsEngine2DLocator
	{
	public:
		static PhysicsEngine2D* GetPhysicsEngine2D()
		{
			return physicsEngine2DInstance;
		}

		static void Provide(PhysicsEngine2D* physicsEngine2D)
		{
			physicsEngine2DInstance = physicsEngine2D;
		}

	private:
		inline static PhysicsEngine2D* physicsEngine2DInstance{ nullptr };
	};
}
