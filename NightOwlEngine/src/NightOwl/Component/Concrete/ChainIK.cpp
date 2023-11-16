#include "NightOwlPch.h"

#include "ChainIK.h"
#include "Transform.h"
#include "NightOwl/Animation/3D/System/AnimatorSystem.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Locator/DebugSystemLocator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	ChainIK::ChainIK()
		: Component(ComponentType::ChainIK),
		  weight(1.0f),
		  totalChainLength(0.0f)
	{
		AnimatorSystemLocator::GetAnimatorSystem()->AddChainIk(this);
	}

	void ChainIK::Update()
	{
		// no need to update if there is only one transform that makes up the chain
		if (chain.size() < 2 && target != nullptr)
		{
			return;
		}

		FabrikSolver();
	}

	void ChainIK::AddJointToChain(Transform* joint)
	{
		chain.push_back(joint);
	}

	void ChainIK::RemoveBackJoint()
	{
		if (chain.size() > 2)
		{
			Transform* lastJoint = chain.back();
			Transform* secondToLastJoint = chain[chain.size() - 2];
			totalChainLength -= (lastJoint->GetPosition() - secondToLastJoint->GetPosition()).Magnitude();
		}

		chain.pop_back();
	}

	float ChainIK::GetWeight()
	{
		return weight;
	}

	void ChainIK::SetWeight(float weight)
	{
		this->weight = weight;
	}

	void ChainIK::SetTarget(Transform* target)
	{
		this->target = target;
	}

	void ChainIK::FabrikSolver()
	{
		static Vec3F prevousPos = target->GetPosition();
		if ((target->GetPosition() - prevousPos).SquareMagnitude() < EPSILON)
		{
			return;
		}
		prevousPos = target->GetPosition();

		std::vector<float> lengths(chain.size() - 1);
		std::vector<Vec3F> points(chain.size());
		for (int jointIndex = 0; jointIndex < chain.size(); ++jointIndex)
		{
			points[jointIndex] = chain[jointIndex]->GetPosition();

			if (jointIndex < chain.size() - 1)
			{
				lengths[jointIndex] = (points[jointIndex] - chain[jointIndex + 1]->GetPosition()).Magnitude();
			}
		}
		
		for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration)
		{
			Backward(points, lengths);
			Forward(points, lengths);
		}

		std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		auto& boneInfoMap = mesh->GetBoneInfoMap();
		QuatF accumulatedBindPose = boneInfoMap.at(chain[0]->GetGameObject().GetName()).offsetRotation.GetInverse();
		for (int jointIndex = 0; jointIndex < points.size() - 1; ++jointIndex)
		{
			Vec3F previousJoint = points[jointIndex];
			Vec3F currentJoint = points[jointIndex + 1];
		
			Vec3F previousToCurrentDirection = (currentJoint - previousJoint).GetNormalize();
		
			QuatF currentBindPoseOrientation = boneInfoMap.at(chain[jointIndex]->GetGameObject().GetName()).offsetRotation.GetInverse();
			QuatF offset;
			if(jointIndex < points.size() - 2)
			{
				QuatF nextBindPoseOrientation = boneInfoMap.at(chain[jointIndex + 1]->GetGameObject().GetName()).offsetRotation.GetInverse();
				offset = currentBindPoseOrientation.GetInverse() * nextBindPoseOrientation;
			}
		
			Vec3F dir = (accumulatedBindPose * offset) * Vec3F::Up();
		
			QuatF toLocalFromBindPose = accumulatedBindPose.GetInverse();
			QuatF rotation = QuatF::FromToRotation(toLocalFromBindPose * dir, toLocalFromBindPose * previousToCurrentDirection);
		
			QuatF parentBindPoseOrientation = boneInfoMap.at(chain[jointIndex]->GetParent().GetGameObject().GetName()).offsetRotation;
			QuatF localBindPoseOffset = parentBindPoseOrientation * currentBindPoseOrientation;
		
			accumulatedBindPose = accumulatedBindPose * rotation;
			if (jointIndex == 0)
			{
				chain[jointIndex]->SetLocalRotation(localBindPoseOffset * rotation);
			}
			else
			{
				chain[jointIndex]->SetLocalRotation(rotation);
			}
		
			chain[jointIndex]->GetWorldMatrix();
			for (int childIndex = 0; childIndex < chain[jointIndex]->GetNumberOfChildren(); childIndex++)
			{
				Transform* childTransform = chain[jointIndex]->GetChildAtIndex(childIndex);
				childTransform->PropagateParentTransform(chain[jointIndex]->worldVecQuatMat);
			}
		}
	}

	void ChainIK::Forward(std::vector<Vec3F>& points, std::vector<float>& lengths)
	{
		std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		auto& boneInfoMap = mesh->GetBoneInfoMap();

		points.front() = chain.front()->GetPosition();
		for (int jointIndex = 0; jointIndex < points.size() - 1; ++jointIndex)
		{
			Vec3F current = points[jointIndex];
			Vec3F next = points[jointIndex + 1];

			Vec3F previousToCurrent = (next - current).GetNormalize() * lengths[jointIndex];

			Vec3F targetPoint = current + previousToCurrent;

			Vec3F previous;
			if (jointIndex == 0)
			{
				previous = current - (current - chain[jointIndex]->GetParent().GetPosition());// boneInfoMap.at(chain[jointIndex]->GetParent().GetGameObject().GetName()).offsetRotation.GetInverse() * Vec3F::Up();
				DebugSystemLocator::GetDebugSystem()->DrawLine(current, chain[jointIndex]->GetParent().GetPosition());
				DebugSystemLocator::GetDebugSystem()->DrawPoint(chain[jointIndex]->GetPosition(), { 1, 0, 0 });
				// DebugSystemLocator::GetDebugSystem()->DrawPoint(current, { 1, 1, 0 });
				DebugSystemLocator::GetDebugSystem()->DrawPoint(chain[jointIndex]->GetParent().GetPosition(), { 1, 1, 1 });

			}
			else
			{
				previous = points[jointIndex - 1];
			}

			Vec3F constraints = Constrain(previous, current, targetPoint, boneInfoMap.at(chain[jointIndex]->GetGameObject().GetName()).offsetRotation.GetInverse(), lengths[jointIndex], true);


			points[jointIndex + 1] = constraints;
		}
	}

	void ChainIK::Backward(std::vector<Vec3F>& points, std::vector<float>& lengths)
	{
		std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		auto& boneInfoMap = mesh->GetBoneInfoMap();

		points.back() = target->GetPosition();
		for (int jointIndex = points.size() - 1; jointIndex > 0; --jointIndex)
		{
			Vec3F current = points[jointIndex];
			Vec3F next = points[jointIndex - 1];

			Vec3F previousToCurrent = (next - current).GetNormalize() * lengths[jointIndex - 1];

			Vec3F targetPoint = current + previousToCurrent;

			if (jointIndex == points.size() - 1)
			{
				points[jointIndex - 1] = targetPoint;
				continue;
			}

			Vec3F previous = points[jointIndex + 1];

			Vec3F constraints = Constrain(previous, current, targetPoint, boneInfoMap.at(chain[jointIndex]->GetGameObject().GetName()).offsetRotation.GetInverse(), lengths[jointIndex - 1]);

			points[jointIndex - 1] = constraints;
		}
	}

	Vec3F ChainIK::Constrain(const Vec3F& previousPoint, const Vec3F& currentPoint, const Vec3F& targetPoint, const QuatF& currentJointOrientation, float jointLength, bool isBack)
	{
		// calculate unit line from previous to current point
		Vec3F unitPreviousToCurrent = (currentPoint - previousPoint).Normalize();

		// calculate direction to target
		Vec3F targetDirection = targetPoint - currentPoint;

		// calculate distance from current point to center of cone
		float distanceToCenterOfCone = Vec3F::Dot(unitPreviousToCurrent, targetDirection);

		// Target is behind the current point, must stretch to max limit
		// Need to determine quadrant to grab proper angle
		if (distanceToCenterOfCone < 0.0f)
		{
			QuatF correctionRotation(Vec3F::Cross(unitPreviousToCurrent, targetDirection).Normalize(), 90.0f);
			return currentPoint + correctionRotation * unitPreviousToCurrent * jointLength;
		}

		// calculate center of cone from current point to target using projection
		Vec3F centerOfCone = currentPoint + unitPreviousToCurrent * distanceToCenterOfCone;

		// calculate lengths of ellipsoidal bounds on the conical region of the cone
		float ellipsoidalNegativeZ = distanceToCenterOfCone * std::tan(DegreesToRad(90.0f)); // replace 50 with actual angle constraints
		float ellipsoidalPositiveZ = distanceToCenterOfCone * std::tan(DegreesToRad(90.0f));
		float ellipsoidalNegativeX = distanceToCenterOfCone * std::tan(DegreesToRad(90.0f));
		float ellipsoidalPositiveX = distanceToCenterOfCone * std::tan(DegreesToRad(90.0f));

		// get target position in cross section of the cone
		Vec3F centerOfConeToTarget = targetPoint - centerOfCone;
		Vec3F unitCenterOfConeToTarget = centerOfConeToTarget.GetNormalize();

		// joint reference axes for cross section of cone
		QuatF toRotation = QuatF::FromToRotation(currentJointOrientation * Vec3F::Up(), unitPreviousToCurrent * -1.0f);
		Vec3F zReferenceAxis = toRotation * Vec3F::Forward();
		Vec3F yReferenceAxis = toRotation * Vec3F::Up();

		// get components of center of cone to target direction vector using reference axes
		float upComponent = Vec3F::Dot(centerOfConeToTarget, yReferenceAxis);
		float rightComponent = Vec3F::Dot(centerOfConeToTarget, zReferenceAxis);

		// get quadrant target is located within the cone cross section
		int quadrant;
		if (upComponent > 0.0f && rightComponent >= 0.0f)
		{
			quadrant = 1;
		}
		else if (upComponent >= 0.0f && rightComponent < 0.0f)
		{
			quadrant = 2;
		}
		else if (upComponent < 0.0f && rightComponent <= 0.0f)
		{
			quadrant = 3;
		}
		else if (upComponent <= 0.0f && rightComponent > 0.0f)
		{
			quadrant = 4;
		}

		// check if target is in cross section of cone
		bool isInConeCrossSection = false;
		if (NearEquals(upComponent, 0.0f) && NearEquals(rightComponent, 0.0f))
		{
			isInConeCrossSection = true;
		}
		else if (quadrant == 1 && EvaluateEllipse(rightComponent, upComponent, ellipsoidalPositiveX, ellipsoidalPositiveZ) < 1.0f)
		{
			isInConeCrossSection = true;
		}
		else if (quadrant == 2 && EvaluateEllipse(rightComponent, upComponent, ellipsoidalNegativeX, ellipsoidalPositiveZ) < 1.0f)
		{
			isInConeCrossSection = true;
		}
		else if (quadrant == 3 && EvaluateEllipse(rightComponent, upComponent, ellipsoidalNegativeX, ellipsoidalNegativeZ) < 1.0f)
		{
			isInConeCrossSection = true;
		}
		else if (quadrant == 4 && EvaluateEllipse(rightComponent, upComponent, ellipsoidalPositiveX, ellipsoidalNegativeZ) < 1.0f)
		{
			isInConeCrossSection = true;
		}

		if (isInConeCrossSection)
		{
			if (Vec3F::Dot(unitPreviousToCurrent, targetDirection) < 0.0f)
			{
				float distanceFromConeCenterToTarget = Vec2F(rightComponent, upComponent).Magnitude();
				return currentPoint + (targetDirection.Negate() + 2.0f * unitCenterOfConeToTarget * distanceFromConeCenterToTarget);
			}

			return  targetPoint;
		}

		// Target is outside of cross section, get nearest point on ellipsoidal region of cross section
		Vec2F nearestPoint;
		if (quadrant == 1)
		{
			nearestPoint.x = ellipsoidalPositiveX;
			if (NearEquals(upComponent, 0.0f) == false)
			{
				nearestPoint = FindNearestPointOnEllipsoid(rightComponent, upComponent, ellipsoidalPositiveX, ellipsoidalPositiveZ);
			}
		}
		else if (quadrant == 2)
		{
			nearestPoint.y = ellipsoidalPositiveZ;
			if (NearEquals(rightComponent, 0.0f) == false)
			{
				nearestPoint = FindNearestPointOnEllipsoid(rightComponent, upComponent, ellipsoidalNegativeX, ellipsoidalPositiveZ);
			}
		}
		else if (quadrant == 3)
		{
			nearestPoint.x = ellipsoidalNegativeX;
			if (NearEquals(upComponent, 0.0f) == false)
			{
				nearestPoint = FindNearestPointOnEllipsoid(rightComponent, upComponent, ellipsoidalNegativeX, ellipsoidalNegativeZ);
			}
		}
		else if (quadrant == 4)
		{
			nearestPoint.x = ellipsoidalNegativeX;
			if (NearEquals(rightComponent, 0.0f) == false)
			{
				nearestPoint = FindNearestPointOnEllipsoid(rightComponent, upComponent, ellipsoidalPositiveX, ellipsoidalNegativeZ);
			}
		}

		float distanceToNearestPoint = nearestPoint.Magnitude();
		Vec3F nearestPointOnEllipsoid = centerOfCone + unitCenterOfConeToTarget * distanceToNearestPoint;
		Vec3F currentToNearestEllipsoidalPoint = nearestPointOnEllipsoid - currentPoint;
		Vec3F updatedTargetPoint = currentPoint + currentToNearestEllipsoidalPoint.Normalize() * jointLength;
		return updatedTargetPoint;
	}

	float ChainIK::EvaluateEllipse(float x, float y, float a, float b)
	{
		const float leftTerm = (x * x) / (a * a);
		const float rightTerm = (y * y) / (b * b);
		return leftTerm + rightTerm;
	}

	Vec2F ChainIK::FindNearestPointOnEllipsoid(float x, float y, float a, float b) const
	{
		const float scaleFactor = 1.0f / std::sqrt(EvaluateEllipse(x, y, a, b));
		const float yNearest = scaleFactor * y;
		const float xNearest = scaleFactor * x;

		return Vec2F(xNearest, yNearest);
	}

	void ChainIK::Remove()
	{
	}
}
