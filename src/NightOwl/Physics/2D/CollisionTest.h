#pragma once

#include "Collider/Collider2D.h"
#include "Collider/AABBBoxCollider2D.h"
#include "Collider/CircleCollider2D.h"
#include "Collider/OBBBoxCollider2D.h"

namespace NightOwl::Physics
{
	class CollisionTest
	{
	public:
		static bool TestCollision(Collider2D* collider, Collider2D* otherCollider);

	private:
		static bool TestAABBAABB(AABBBoxCollider2D* collider, AABBBoxCollider2D* otherCollider);

		static bool TestCircleCircle(CircleCollider2D* collider, CircleCollider2D* otherCollider);

		static bool TestAABBCircle(AABBBoxCollider2D* collider, CircleCollider2D* otherCollider);

		static bool TestOBBOBB(OBBBoxCollider2D* collider, OBBBoxCollider2D* otherCollider);

		static bool TestAABBOBB(AABBBoxCollider2D* collider, OBBBoxCollider2D* otherCollider);

		static bool TestCircleOBB(CircleCollider2D* collider, OBBBoxCollider2D* otherCollider);
	};
}
