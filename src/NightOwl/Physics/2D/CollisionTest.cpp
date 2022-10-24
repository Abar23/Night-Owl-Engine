#include "CollisionTest.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"
#include <algorithm>
#include <cmath>

namespace NightOwl::Physics
{
	bool CollisionTest::TestCollision(Collider2D* collider, Collider2D* otherCollider)
	{
		if (dynamic_cast<AABBBoxCollider2D*>(collider))
		{
			if(dynamic_cast<AABBBoxCollider2D*>(otherCollider))
			{
				return TestAABBAABB(dynamic_cast<AABBBoxCollider2D*>(collider), dynamic_cast<AABBBoxCollider2D*>(otherCollider));
			}

			if(dynamic_cast<CircleCollider2D*>(otherCollider))
			{
				return TestAABBCircle(dynamic_cast<AABBBoxCollider2D*>(collider), dynamic_cast<CircleCollider2D*>(otherCollider));
			}
		}

		if (dynamic_cast<CircleCollider2D*>(collider))
		{
			if(dynamic_cast<CircleCollider2D*>(otherCollider))
			{
				return TestCircleCircle(dynamic_cast<CircleCollider2D*>(collider), dynamic_cast<CircleCollider2D*>(otherCollider));
			}

			if (dynamic_cast<AABBBoxCollider2D*>(otherCollider))
			{
				return TestAABBCircle(dynamic_cast<AABBBoxCollider2D*>(otherCollider), dynamic_cast<CircleCollider2D*>(collider));
			}
		}

		return false;
	}

	bool CollisionTest::TestAABBAABB(AABBBoxCollider2D* collider, AABBBoxCollider2D* otherCollider)
	{
		Math::Vec2F colliderHalfWidths = collider->GetHalfWidths();
		Math::Vec2F colliderCenter = collider->GetCenterOfBoundingBox();

		Math::Vec2F otherColliderHalfWidths = otherCollider->GetHalfWidths();
		Math::Vec2F otherColliderCenter = otherCollider->GetCenterOfBoundingBox();

		if(std::abs(colliderCenter.x - otherColliderCenter.x) > colliderHalfWidths.x + otherColliderHalfWidths.x)
		{
			return false;
		}

		if (std::abs(colliderCenter.y - otherColliderCenter.y) > colliderHalfWidths.y + otherColliderHalfWidths.y)
		{
			return false;
		}

		return true;
	}

	bool CollisionTest::TestCircleCircle(CircleCollider2D* collider, CircleCollider2D* otherCollider)
	{
		const Math::Vec2F distanceBetweenCenters = collider->GetCenterOfCircle() - otherCollider->GetCenterOfCircle();

		const float squaredDistance = Math::Vec2F::Dot(distanceBetweenCenters, distanceBetweenCenters);

		const float sumOfRadiiSquared = otherCollider->GetRadius() + otherCollider->GetRadius() * otherCollider->GetRadius() + otherCollider->GetRadius();

		return squaredDistance < sumOfRadiiSquared || Math::NearEquals(squaredDistance, sumOfRadiiSquared);
	}

	// Nice algorithm here: https://arrowinmyknee.com/2021/04/02/how-to-detect-intersection-between-circle-and-aabb/
	bool CollisionTest::TestAABBCircle(AABBBoxCollider2D* collider, CircleCollider2D* otherCollider)
	{
		// Get distance between circle and aabb box
		Math::Vec2F distanceFromBoxToCirlce = collider->GetCenterOfBoundingBox() - otherCollider->GetCenterOfCircle();

		// Force vector into into first quadrant. Imagine circle being in the first quadrant
		distanceFromBoxToCirlce.x = std::abs(distanceFromBoxToCirlce.x);
		distanceFromBoxToCirlce.y = std::abs(distanceFromBoxToCirlce.y);

		// Get distance between corner of aabb box and center of circle
		Math::Vec2F distanceTopRightCornerToCircle = distanceFromBoxToCirlce - collider->GetHalfWidths();
		distanceTopRightCornerToCircle.x = std::max(distanceTopRightCornerToCircle.x, 0.0f);
		distanceTopRightCornerToCircle.y = std::max(distanceTopRightCornerToCircle.y, 0.0f);

		return Math::Vec2F::Dot(distanceTopRightCornerToCircle, distanceTopRightCornerToCircle) < otherCollider->GetRadius() * otherCollider->GetRadius();
	}
}
