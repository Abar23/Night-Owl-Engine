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

		if(dynamic_cast<OBBBoxCollider2D*>(collider))
		{
			if (dynamic_cast<OBBBoxCollider2D*>(otherCollider))
			{
				return TestOBBOBB(dynamic_cast<OBBBoxCollider2D*>(collider), dynamic_cast<OBBBoxCollider2D*>(otherCollider));
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

		ENGINE_LOG_INFO("Collision between game object {0} and {1}", collider->GetRigidBody()->GetGameObject().GetName(), otherCollider->GetRigidBody()->GetGameObject().GetName());

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
		Math::Vec2F colliderNormals[2] = {
			colliderRotationMatrix.GetColumn(0).xy * colliderHalfWidths,
			colliderRotationMatrix.GetColumn(1).xy* colliderHalfWidths
		};

		colliderHalfWidths = Math::Vec2F(colliderRotationMatrix.GetColumn(0).x * colliderHalfWidths.x, colliderRotationMatrix.GetColumn(0).y * colliderHalfWidths.y);

		Math::Mat3F otherColliderRotationMatrix = otherCollider->GetOrientation().GetRotationMatrix();
		Math::Vec2F otherColliderCenter = otherCollider->GetCenterOfBoundingBox();
		Math::Vec2F otherColliderHalfWidths = otherCollider->GetHalfWidths();
		otherColliderHalfWidths = Math::Vec2F(otherColliderRotationMatrix.GetColumn(0).x * otherColliderHalfWidths.x, otherColliderRotationMatrix.GetColumn(0).y * otherColliderHalfWidths.y);
		Math::Vec2F otherColliderNormals[2] = {
			otherColliderRotationMatrix.GetColumn(0).xy * otherColliderHalfWidths,
			otherColliderRotationMatrix.GetColumn(1).xy * otherColliderHalfWidths
		};

		// Test object 1 normals
		for (int axisIndex = 0; axisIndex < 2; axisIndex++)
		{
			Math::Vec2F axisOfRotation = colliderNormals[axisIndex];

			float colliderMinPoint = std::numeric_limits<float>::max();
			float colliderMaxPoint = -std::numeric_limits<float>::min();

			float valueAlongAxis = Math::Vec2F::Dot(colliderCenter + colliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter - colliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter + colliderHalfWidths.x - colliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter - colliderHalfWidths.x + colliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;


			float otherColliderMinPoint = std::numeric_limits<float>::max();
			float otherColliderMaxPoint = -std::numeric_limits<float>::max();

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter + otherColliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter - otherColliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter + otherColliderHalfWidths.x - otherColliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter - otherColliderHalfWidths.x + otherColliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			if(!((colliderMinPoint <= otherColliderMinPoint && otherColliderMinPoint <= colliderMaxPoint) || 
				(otherColliderMinPoint <= colliderMinPoint && colliderMinPoint <= otherColliderMaxPoint)))
			{
				return false;
			}
		}

		// test object 2 normals
		for (int axisIndex = 0; axisIndex < 2; axisIndex++)
		{
			Math::Vec2F axisOfRotation = otherColliderNormals[axisIndex];

			float colliderMinPoint = std::numeric_limits<float>::max();
			float colliderMaxPoint = -std::numeric_limits<float>::max();

			float valueAlongAxis = Math::Vec2F::Dot(colliderCenter + colliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter - colliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter + colliderHalfWidths.x - colliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(colliderCenter - colliderHalfWidths.x + colliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < colliderMinPoint)  colliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > colliderMaxPoint)  colliderMaxPoint = valueAlongAxis;


			float otherColliderMinPoint = std::numeric_limits<float>::max();
			float otherColliderMaxPoint = -std::numeric_limits<float>::max();

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter + otherColliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter - otherColliderHalfWidths, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter + otherColliderHalfWidths.x - otherColliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			valueAlongAxis = Math::Vec2F::Dot(otherColliderCenter - otherColliderHalfWidths.x + otherColliderHalfWidths.y, axisOfRotation);
			if (valueAlongAxis < otherColliderMinPoint)  otherColliderMinPoint = valueAlongAxis;
			if (valueAlongAxis > otherColliderMaxPoint)  otherColliderMaxPoint = valueAlongAxis;

			if (!((colliderMinPoint <= otherColliderMinPoint && otherColliderMinPoint <= colliderMaxPoint) ||
				(otherColliderMinPoint <= colliderMinPoint && colliderMinPoint <= otherColliderMaxPoint)))
			{
				return false;
			}
		}

		std::cout << "Collide!" << std::endl;
		return true;
	}
}
