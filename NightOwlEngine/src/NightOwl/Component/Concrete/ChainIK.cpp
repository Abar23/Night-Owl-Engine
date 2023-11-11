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

		// const Vec3F chainStartToTarget = target->GetPosition() - chain.front()->GetPosition();
		// const float targetDistance = (chainStartToTarget).Magnitude();
		// if (targetDistance > totalChainLength)
		// {
		// 	const Vec3F directionToTarget = chainStartToTarget.GetNormalize();
		// 	DebugSystemLocator::GetDebugSystem()->DrawLine(chain.front()->GetPosition(), chain.front()->GetPosition() + chainStartToTarget);
		// 	std::vector<Vec3F> newPositions;
		// 	for (int jointIndex = 1; jointIndex < chain.size(); ++jointIndex)
		// 	{
		// 		Transform* previousJoint = chain[jointIndex - 1];
		// 		Transform* currentJoint = chain[jointIndex];
		//
		// 		Vec3F previousJointPosition = previousJoint->GetPosition();
		// 		Vec3F currentJointPosition = currentJoint->GetPosition();
		// 		DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJointPosition, { 0.0f, 0.0f, 1.0f });
		// 		DebugSystemLocator::GetDebugSystem()->DrawPoint(currentJointPosition, { 0.0f, 0.0f, 1.0f });
		//
		// 		float linkLength = (previousJointPosition - currentJointPosition).Magnitude();
		//
		// 		Vec3F targetPosition = previousJointPosition + directionToTarget * linkLength;
		// 		DebugSystemLocator::GetDebugSystem()->DrawPoint(targetPosition, { 1.0f, 0.0f, 0.0f });
		// 		newPositions.push_back(targetPosition);
		// 	}
		// 	for (int jointIndex = 1; jointIndex < chain.size(); ++jointIndex)
		// 	{
		// 		chain[jointIndex]->SetPosition(newPositions[jointIndex - 1]);
		// 	}
		// }

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
			// backwards
			points.back() = target->GetPosition();
			for (int jointIndex = points.size() - 1; jointIndex > 0; --jointIndex)
			{
				Vec3F previousJoint = points[jointIndex];
				Vec3F currentJoint = points[jointIndex - 1];

				Vec3F previousToCurrent = (currentJoint - previousJoint).GetNormalize() * lengths[jointIndex - 1];

				points[jointIndex - 1] = previousJoint + previousToCurrent;
			}

			// forwards
			points.front() = chain.front()->GetPosition();
			for (int jointIndex = 0; jointIndex < points.size() - 1; ++jointIndex)
			{
				Vec3F previousJoint = points[jointIndex];
				Vec3F currentJoint = points[jointIndex + 1];

				Vec3F previousToCurrentDirection = (currentJoint - previousJoint).GetNormalize();
				Vec3F previousToCurrent = previousToCurrentDirection * lengths[jointIndex];

				points[jointIndex + 1] = previousJoint + previousToCurrent;
			}
		}

		std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		auto& boneInfoMap = mesh->GetBoneInfoMap();
		for (int jointIndex = 0; jointIndex < points.size() - 1; ++jointIndex)
		{
			Vec3F previousJoint = points[jointIndex];
			Vec3F currentJoint = points[jointIndex + 1];

			DebugSystemLocator::GetDebugSystem()->DrawPoint(currentJoint, { 1.0f, 0.0f, 0.0f });
			DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, currentJoint);

			Vec3F previousToCurrentDirection = (currentJoint - chain[jointIndex]->GetPosition()).Normalize();

			Vec3F dir = (chain[jointIndex + 1]->GetPosition() - chain[jointIndex]->GetPosition()).Normalize();

			QuatF bindPostOrientation;
			bindPostOrientation.SetOrthogonalRotationMatrix(boneInfoMap.at(chain[jointIndex]->GetGameObject().GetName()).offsetMatrix.GetRotationMatrix());

			QuatF parentBindPoseOrientation;
			parentBindPoseOrientation.SetOrthogonalRotationMatrix(boneInfoMap.at(chain[jointIndex]->GetParent().GetGameObject().GetName()).offsetMatrix.GetRotationMatrix());

			QuatF toLocal = chain[jointIndex]->GetRotation().GetInverse();
			QuatF rotation = QuatF::FromToRotation(toLocal * dir, toLocal * previousToCurrentDirection);

			chain[jointIndex]->Rotate(rotation, Space::Local);

			if (jointIndex == 0)
			{
				Vec3F angles = chain[jointIndex]->GetLocalRotation().GetEulerAngles();
				angles.x = std::clamp(angles.x, -90.0f, 90.0f);
				angles.y = std::clamp(angles.y, -90.0f, 90.0f);
				angles.z = std::clamp(angles.z, -90.0f, 90.0f);
				QuatF difference = parentBindPoseOrientation * bindPostOrientation.Inverse();
				// angles += difference.GetEulerAngles();
				ENGINE_LOG_INFO("Difference: {0}\n", angles.ToString());
				chain[jointIndex]->SetLocalRotation(QuatF::MakeRotationFromEulers(angles));
			}
			
			chain[jointIndex]->GetWorldMatrix();
			for (int childIndex = 0; childIndex < chain[jointIndex]->GetNumberOfChildren(); childIndex++)
			{
				Transform* childTransform = chain[jointIndex]->GetChildAtIndex(childIndex);
				childTransform->PropagateParentLocalTransform(chain[jointIndex]->worldVecQuatMat);
			}
		}
	}

	void ChainIK::Forward()
	{

	}

	void ChainIK::Backwards()
	{
	}

	void ChainIK::Remove()
	{
	}
}
