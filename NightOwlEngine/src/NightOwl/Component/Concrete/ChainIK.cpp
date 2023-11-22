#include "NightOwlPch.h"

#include "ChainIK.h"
#include "Transform.h"
#include "NightOwl/Animation/3D/Constants.h"
#include "NightOwl/Animation/3D/System/AnimatorSystem.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/Core/Locator/AnimatorSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	ChainIK::ChainIK()
		: Component(ComponentType::ChainIK),
		  weight(1.0f),
		  totalChainLength(0.0f),
		  shouldApplyJointConstraints(false),
		  target(nullptr)
	{
		AnimatorSystemLocator::GetAnimatorSystem()->AddChainIk(this);
	}

	void ChainIK::Start()
	{
		mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		ENGINE_ASSERT(mesh != nullptr, "ChainIK component must be on the same Game Object containing the renderer.");
	}

	void ChainIK::Update()
	{
		// no need to update if there is only one transform that makes up the chain
		if (target != nullptr && chain.size() < 2)
		{
			return;
		}

		const Vec3F currentTargetPosition = target->GetPosition();
		const Vec3F currentPosition = gameObject->GetTransform()->GetPosition();
		if ((currentTargetPosition - previousTargetPosition).SquareMagnitude() < SQUARE_EPSILON &&
			(currentPosition - previousGameObjectPosition).SquareMagnitude() < SQUARE_EPSILON)
		{
			return;
		}
		previousTargetPosition = currentTargetPosition;
		previousGameObjectPosition = currentPosition;

		FabrikSolver();
	}

	std::shared_ptr<Component> ChainIK::Clone()
	{
		std::shared_ptr<ChainIK> clone = std::make_shared<ChainIK>();

		clone->weight = weight;
		clone->totalChainLength = totalChainLength;
		clone->previousTargetPosition = previousTargetPosition;
		clone->previousGameObjectPosition = previousGameObjectPosition;
		clone->target = target;
		clone->mesh = mesh;
		clone->shouldApplyJointConstraints = shouldApplyJointConstraints;
		clone->chain = chain;
		clone->bindPoses = bindPoses;
		clone->parentBoneInfo = parentBoneInfo;

		return clone;
	}

	void ChainIK::SetBallAndSocketConstraintForJoint(int jointIndex, const BallAndSocketConstraint& constraints)
	{
		ENGINE_ASSERT(jointIndex >= 0 && jointIndex < chain.size(), "Invalid joint index for setting constraints.");

		jointBallAndSocketMap[jointIndex] = constraints;
	}

	void ChainIK::AddJointToChain(Transform* joint)
	{
		chain.push_back(joint);

		const BoneInfo& boneInfo = mesh->GetBoneInfoMap().at(joint->gameObject->GetName());
		bindPoses.push_back(&boneInfo);

		if (chain.size() > 1)
		{
			return;
		}

		Transform& parentTransform = joint->gameObject->GetTransform()->GetParent();
		parentBoneInfo = &mesh->GetBoneInfoMap().at(parentTransform.GetGameObject().GetName());
	}

	void ChainIK::RemoveBackJoint()
	{
		if (chain.empty())
		{
			return;
		}

		if (chain.size() > 1)
		{
			totalChainLength -= chainLengths.back();
			chainLengths.pop_back();
		}

		chain.pop_back();
		bindPoses.pop_back();
	}

	float ChainIK::GetWeight() const
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
		if (fabrikPoints.size() != chain.size())
		{
			fabrikPoints.resize(chain.size());
			chainLengths.resize(chain.size() - 1);
		}

		for (size_t jointIndex = 0; jointIndex < chain.size(); ++jointIndex)
		{
			fabrikPoints[jointIndex] = chain[jointIndex]->GetPosition();

			if (jointIndex < chain.size() - 1)
			{
				const float lengthOfJoint = (chain[jointIndex]->GetPosition() - chain[jointIndex + 1]->GetPosition()).Magnitude();
				chainLengths[jointIndex] = lengthOfJoint;
				totalChainLength += lengthOfJoint;
			}
		}

		int numberOfIterations = MAX_FABRIK_ITERATIONS;

		const float distanceToTarget = (target->GetPosition() - chain.front()->GetPosition()).Magnitude();
		if (totalChainLength < distanceToTarget)
		{
			numberOfIterations = 1;
		}

		for (int iteration = 0; iteration < numberOfIterations; ++iteration)
		{
			Backward();
			Forward();

			const Vec3F endEffectorToTarget = target->GetPosition() - chain.back()->GetPosition();
			if (endEffectorToTarget.SquareMagnitude() < FABRIK_TOLERANCE_SQUARED)
			{
				break;
			}
		}
	}

	void ChainIK::ApplySolveToChain()
	{
		if (fabrikPoints.empty())
		{
			return;
		}

		for (size_t jointIndex = 0; jointIndex < fabrikPoints.size() - 1; ++jointIndex)
		{
			Vec3F current = fabrikPoints[jointIndex];
			Vec3F next = fabrikPoints[jointIndex + 1];
		
			Vec3F currentToNextDirection = (next - current).GetNormalize();
		
			Vec3F bindPoseDirection = chain[jointIndex]->GetRotation() * Vec3F::Up();
			QuatF toLocalFromBindPose = chain[jointIndex]->GetRotation().GetInverse();
		
			QuatF rotation = QuatF::RotateFromTo(toLocalFromBindPose * bindPoseDirection, toLocalFromBindPose * currentToNextDirection);
			QuatF localRotation = chain[jointIndex]->GetLocalRotation() * rotation;
			localRotation = QuatF::Slerp(chain[jointIndex]->GetLocalRotation(), localRotation, weight);
		
			chain[jointIndex]->SetLocalRotation(localRotation.Normalize());
			chain[jointIndex]->GetWorldMatrix();
		
			for (int childIndex = 0; childIndex < chain[jointIndex]->GetNumberOfChildren(); childIndex++)
			{
				Transform* childTransform = chain[jointIndex]->GetChildAtIndex(childIndex);
				childTransform->PropagateParentTransform(chain[jointIndex]->worldVecQuatMat);
			}
		}
	}

	void ChainIK::EnableConstraints(bool enable)
	{
		shouldApplyJointConstraints = enable;
	}

	bool ChainIK::AreConstraintsEnabled() const
	{
		return shouldApplyJointConstraints;
	}

	void ChainIK::ForceSolve()
	{
		FabrikSolver();
	}

	void ChainIK::Forward()
	{
		fabrikPoints.front() = chain.front()->GetPosition();
		for (size_t jointIndex = 0; jointIndex < fabrikPoints.size() - 1; ++jointIndex)
		{
			Vec3F current = fabrikPoints[jointIndex];
			Vec3F next = fabrikPoints[jointIndex + 1];

			Vec3F currentToNext = (next - current).GetNormalize() * chainLengths[jointIndex];

			Vec3F targetPoint = current + currentToNext;

			Vec3F previous;
			if (jointIndex == 0)
			{
				previous = current + (chain[jointIndex]->GetParent().GetPosition() - current);
			
			}
			else
			{
				previous = fabrikPoints[jointIndex - 1];
			}

			Vec3F constraints = targetPoint;
			if (shouldApplyJointConstraints)
			{
				if (jointBallAndSocketMap.contains(jointIndex))
				{
					constraints = ConstrainBallAndSocket(previous, current, targetPoint, chain[jointIndex]->GetParent().GetRotation(), chainLengths[jointIndex], jointIndex);
				}
			}
			
			fabrikPoints[jointIndex + 1] = constraints;
		}
	}

	void ChainIK::Backward()
	{
		fabrikPoints.back() = target->GetPosition();
		for (size_t jointIndex = fabrikPoints.size() - 1; jointIndex > 0; --jointIndex)
		{
			Vec3F current = fabrikPoints[jointIndex];
			Vec3F next = fabrikPoints[jointIndex - 1];

			Vec3F currentToNext = (next - current).GetNormalize() * chainLengths[jointIndex - 1];

			Vec3F targetPoint = current + currentToNext;

			if (jointIndex == fabrikPoints.size() - 1)
			{
				fabrikPoints[jointIndex - 1] = targetPoint;
				continue;
			}

			Vec3F previous = fabrikPoints[jointIndex + 1];

			Vec3F constraints = targetPoint;
			if (shouldApplyJointConstraints)
			{
				if (jointBallAndSocketMap.contains(jointIndex))
				{
					constraints = ConstrainBallAndSocket(previous, current, targetPoint, chain[jointIndex]->GetParent().GetRotation(), chainLengths[jointIndex], jointIndex);
				}
			}

			fabrikPoints[jointIndex - 1] = constraints;
		}
	}

	Vec3F ChainIK::ConstrainBallAndSocket(const Vec3F& previousPoint, 
										  const Vec3F& currentPoint,
										  const Vec3F& targetPoint, 
										  const QuatF& currentJointOrientation, 
										  float jointLength,
										  int jointIndex) const
	{
		// Ellipsoidal ball and socket
		const BallAndSocketConstraint& constraints = jointBallAndSocketMap.at(jointIndex);

		// calculate unit line from previous to current point
		Vec3F unitPreviousToCurrent = (currentPoint - previousPoint).Normalize();

		// calculate direction to target
		Vec3F targetDirection = targetPoint - currentPoint;

		// calculate distance from current point to center of cone
		float distanceToCenterOfCone = Vec3F::Dot(unitPreviousToCurrent, targetDirection);

		Vec3F updatedTargetPoint = targetPoint;
		if (distanceToCenterOfCone < 0.0f)
		{
			Vec3F projectionAlongConeCenter = Vec3F::Project(targetPoint - currentPoint, unitPreviousToCurrent);
			updatedTargetPoint -= 2.0f * projectionAlongConeCenter;
			distanceToCenterOfCone *= -1.0f;
			targetDirection = updatedTargetPoint - currentPoint;
		}

		if (NearEquals(distanceToCenterOfCone, 0.0f))
		{
			// don't allow this value to go zero, otherwise will produce nan values down below
			distanceToCenterOfCone = 0.1f;
		}

		// calculate center of cone from current point to target using projection
		Vec3F centerOfCone = currentPoint + unitPreviousToCurrent * distanceToCenterOfCone;

		// calculate lengths of ellipsoidal bounds on the conical region of the cone
		float ellipsoidalNegativeZ = distanceToCenterOfCone * std::tan(DegreesToRad(constraints.zAxisJointAngles.min));
		float ellipsoidalPositiveZ = distanceToCenterOfCone * std::tan(DegreesToRad(constraints.zAxisJointAngles.max));
		float ellipsoidalNegativeX = distanceToCenterOfCone * std::tan(DegreesToRad(constraints.xAxisJointAngles.min));
		float ellipsoidalPositiveX = distanceToCenterOfCone * std::tan(DegreesToRad(constraints.xAxisJointAngles.max));

		// get target position in cross section of the cone
		Vec3F centerOfConeToTarget = updatedTargetPoint - centerOfCone;
		Vec3F unitCenterOfConeToTarget = centerOfConeToTarget.GetNormalize();

		// joint reference axes for cross section of cone
		QuatF toRotation = QuatF::RotateFromTo(currentJointOrientation * Vec3F::Up(), unitPreviousToCurrent * -1.0f);
		Vec3F zReferenceAxis = toRotation * Vec3F::Forward();
		Vec3F yReferenceAxis = toRotation * Vec3F::Right();

		// get components of center of cone to target direction vector using reference axes
		float upComponent = Vec3F::Dot(centerOfConeToTarget, yReferenceAxis);
		float rightComponent = Vec3F::Dot(centerOfConeToTarget, zReferenceAxis);

		// get quadrant target is located within the cone cross section
		int quadrant = 1;
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

		// Target is behind the current point, must stretch to max limit
		// Need to determine quadrant to grab proper angle
		bool isBehindConeCrossSection = false;
		if (distanceToCenterOfCone < 0.0f)
		{
			isBehindConeCrossSection = true;
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

		if (isInConeCrossSection && isBehindConeCrossSection == false)
		{
			if (Vec3F::Dot(unitPreviousToCurrent, targetDirection) < 0.0f)
			{
				float distanceFromConeCenterToTarget = Vec2F(rightComponent, upComponent).Magnitude();
				return currentPoint + (targetDirection.Negate() + 2.0f * unitCenterOfConeToTarget * distanceFromConeCenterToTarget);
			}

			return  updatedTargetPoint;
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
		updatedTargetPoint = currentPoint + currentToNearestEllipsoidalPoint.GetNormalize() * jointLength;

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
		AnimatorSystemLocator::GetAnimatorSystem()->RemoveChainIk(this);
	}
}
