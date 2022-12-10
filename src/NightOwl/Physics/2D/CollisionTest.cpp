#include "CollisionTest.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/GameObject/GameObject.h"
#include <limits>
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

			if (dynamic_cast<OBBBoxCollider2D*>(otherCollider))
			{
				return TestAABBOBB(dynamic_cast<AABBBoxCollider2D*>(collider), dynamic_cast<OBBBoxCollider2D*>(otherCollider));
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

			if (dynamic_cast<OBBBoxCollider2D*>(otherCollider))
			{
				return TestCircleOBB(dynamic_cast<CircleCollider2D*>(collider), dynamic_cast<OBBBoxCollider2D*>(otherCollider));
			}
		}

		if(dynamic_cast<OBBBoxCollider2D*>(collider))
		{
			if (dynamic_cast<OBBBoxCollider2D*>(otherCollider))
			{
				return TestOBBOBB(dynamic_cast<OBBBoxCollider2D*>(collider), dynamic_cast<OBBBoxCollider2D*>(otherCollider));
			}

			if(dynamic_cast<AABBBoxCollider2D*>(otherCollider))
			{
				return TestAABBOBB(dynamic_cast<AABBBoxCollider2D*>(otherCollider), dynamic_cast<OBBBoxCollider2D*>(collider));
			}

			if (dynamic_cast<CircleCollider2D*>(otherCollider))
			{
				return TestCircleOBB(dynamic_cast<CircleCollider2D*>(otherCollider), dynamic_cast<OBBBoxCollider2D*>(collider));
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

	// Needs to be cleaned up
	bool CollisionTest::TestOBBOBB(OBBBoxCollider2D* collider, OBBBoxCollider2D* otherCollider)
	{
		Math::Mat3F colliderRotationMatrix = collider->GetOrientation().GetRotationMatrix();
		Math::Vec2F colliderCenter = collider->GetCenterOfBoundingBox();
		Math::Vec2F colliderHalfWidths = collider->GetHalfWidths();

		Math::Mat3F otherColliderRotationMatrix = otherCollider->GetOrientation().GetRotationMatrix();
		Math::Vec2F otherColliderCenter = otherCollider->GetCenterOfBoundingBox();
		Math::Vec2F otherColliderHalfWidths = otherCollider->GetHalfWidths();

		Math::Vec2F axesToTest[4] = {
			colliderRotationMatrix.GetColumn(0).xy,
			colliderRotationMatrix.GetColumn(1).xy,
			otherColliderRotationMatrix.GetColumn(0).xy,
			otherColliderRotationMatrix.GetColumn(1).xy
		};

		Math::Vec2F Vx = axesToTest[0] * colliderHalfWidths.x;
		Math::Vec2F Vy = axesToTest[1] * colliderHalfWidths.y;
		Math::Vec2F otherVx = axesToTest[2] * otherColliderHalfWidths.x;
		Math::Vec2F otherVy = axesToTest[3] * otherColliderHalfWidths.y;

		Math::Vec2F colliderPoints[4] = {
			colliderCenter + Vx + Vy,
			colliderCenter - Vx - Vy,
			colliderCenter + Vx - Vy,
			colliderCenter - Vx + Vy
		};

		Math::Vec2F otherColliderPoints[4] = {
			otherColliderCenter + otherVx + otherVy,
			otherColliderCenter - otherVx - otherVy,
			otherColliderCenter + otherVx - otherVy,
			otherColliderCenter - otherVx + otherVy
		};

		for (int axisIndex = 0; axisIndex < 4; axisIndex++)
		{
			float colliderMinPoint = std::numeric_limits<float>::max();
			float colliderMaxPoint = -std::numeric_limits<float>::max();

			for (int pointIndex = 0; pointIndex < 4; pointIndex++)
			{
				float valueAlongAxis = Math::Vec2F::Dot(colliderPoints[pointIndex], axesToTest[axisIndex]);
				if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
				if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;
			}

			float otherColliderMinPoint = std::numeric_limits<float>::max();
			float otherColliderMaxPoint = -std::numeric_limits<float>::max();

			for (int pointIndex = 0; pointIndex < 4; pointIndex++)
			{
				float valueAlongAxis = Math::Vec2F::Dot(otherColliderPoints[pointIndex], axesToTest[axisIndex]);
				if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
				if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;
			}

			if (!((colliderMinPoint <= otherColliderMinPoint && otherColliderMinPoint <= colliderMaxPoint) ||
				(otherColliderMinPoint <= colliderMinPoint && colliderMinPoint <= otherColliderMaxPoint)))
			{
				return false;
			}
		}

		return true;
	}

	bool CollisionTest::TestAABBOBB(AABBBoxCollider2D* collider, OBBBoxCollider2D* otherCollider)
	{
		Math::Vec2F colliderHalfWidths = collider->GetHalfWidths();
		Math::Vec2F colliderCenter = collider->GetCenterOfBoundingBox();

		Math::Mat3F otherColliderRotationMatrix = otherCollider->GetOrientation().GetRotationMatrix();
		Math::Vec2F otherColliderCenter = otherCollider->GetCenterOfBoundingBox();
		Math::Vec2F otherColliderHalfWidths = otherCollider->GetHalfWidths();

		Math::Vec2F axesToTest[4] = {
			Math::Vec2F::Right(),
			Math::Vec2F::Up(),
			otherColliderRotationMatrix.GetColumn(0).xy,
			otherColliderRotationMatrix.GetColumn(1).xy
		};

		Math::Vec2F otherVx = axesToTest[2] * otherColliderHalfWidths.x;
		Math::Vec2F otherVy = axesToTest[3] * otherColliderHalfWidths.y;

		Math::Vec2F colliderPoints[4] = {
			colliderCenter + colliderHalfWidths,
			colliderCenter + colliderHalfWidths * -1.0f,
			colliderCenter + Math::Vec2F(-colliderHalfWidths.x, colliderHalfWidths.y),
			colliderCenter + Math::Vec2F(colliderHalfWidths.x, -colliderHalfWidths.y)
		};

		Math::Vec2F otherColliderPoints[4] = {
			otherColliderCenter + otherVx + otherVy,
			otherColliderCenter - otherVx - otherVy,
			otherColliderCenter + otherVx - otherVy,
			otherColliderCenter - otherVx + otherVy
		};

		for (int axisIndex = 0; axisIndex < 4; axisIndex++)
		{
			float colliderMinPoint = std::numeric_limits<float>::max();
			float colliderMaxPoint = -std::numeric_limits<float>::max();

			for (int pointIndex = 0; pointIndex < 4; pointIndex++)
			{
				float valueAlongAxis = Math::Vec2F::Dot(colliderPoints[pointIndex], axesToTest[axisIndex]);
				if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
				if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;
			}

			float otherColliderMinPoint = std::numeric_limits<float>::max();
			float otherColliderMaxPoint = -std::numeric_limits<float>::max();

			for (int pointIndex = 0; pointIndex < 4; pointIndex++)
			{
				float valueAlongAxis = Math::Vec2F::Dot(otherColliderPoints[pointIndex], axesToTest[axisIndex]);
				if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
				if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;
			}

			if (!((colliderMinPoint <= otherColliderMinPoint && otherColliderMinPoint <= colliderMaxPoint) ||
				(otherColliderMinPoint <= colliderMinPoint && colliderMinPoint <= otherColliderMaxPoint)))
			{
				return false;
			}
		}
		return true;
	}

	bool CollisionTest::TestCircleOBB(CircleCollider2D* collider, OBBBoxCollider2D* otherCollider)
	{
		// Rotate center of circle by obb orientation
		Math::Vec2F circleColliderCenter = collider->GetCenterOfCircle() - otherCollider->GetCenterOfBoundingBox();
		Math::Vec2F rotatedCircleCenter = Math::QuatF::TransformVector(otherCollider->GetOrientation().GetInverse(), Math::Vec3F(circleColliderCenter.x, circleColliderCenter.y, 0.0f)).xy;

		// Get distance between circle and obb box and force vector into into first quadrant. Imagine circle being in the first quadrant
		Math::Vec2F distanceFromBoxToCircle;
		distanceFromBoxToCircle.x = std::abs(rotatedCircleCenter.x);
		distanceFromBoxToCircle.y = std::abs(rotatedCircleCenter.y);

		// Get distance between corner of obb box and center of circle
		Math::Vec2F distanceTopRightCornerToCircle = distanceFromBoxToCircle - otherCollider->GetHalfWidths();
		distanceTopRightCornerToCircle.x = std::max(distanceTopRightCornerToCircle.x, 0.0f);
		distanceTopRightCornerToCircle.y = std::max(distanceTopRightCornerToCircle.y, 0.0f);

		return Math::Vec2F::Dot(distanceTopRightCornerToCircle, distanceTopRightCornerToCircle) < collider->GetRadius() * collider->GetRadius();
	}
}
