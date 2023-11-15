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
		if ((target->GetPosition() - chain.back()->GetPosition()).Magnitude() < 0.01f)
		{
			return;
		}

		// static int getInfo = 0;
		// static std::vector<float> lengths(chain.size() - 1);
		// static std::vector<Vec3F> originalPoints(chain.size());
		// if (getInfo == 0)
		// {
		// 	getInfo = 1;
		// 	for (int jointIndex = 0; jointIndex < chain.size(); ++jointIndex)
		// 	{
		// 		originalPoints[jointIndex] = chain[jointIndex]->GetPosition();
		//
		// 		if (jointIndex < chain.size() - 1)
		// 		{
		// 			lengths[jointIndex] = (originalPoints[jointIndex] - chain[jointIndex + 1]->GetPosition()).Magnitude();
		// 		}
		// 	}
		// }
		//
		// std::vector<Vec3F> points = originalPoints;

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

		for (int iteration = 0; iteration < 25; ++iteration)
		{
			// backwards
			points.back() = target->GetPosition();
			for (int jointIndex = points.size() - 1; jointIndex > 0; --jointIndex)
			{
				Vec3F previousJoint = points[jointIndex];
				Vec3F currentJoint = points[jointIndex - 1];

				Vec3F previousToCurrent = (currentJoint - previousJoint).GetNormalize() * lengths[jointIndex - 1];

				DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + previousToCurrent);
				DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJoint, { 1.0f, 0.0f, 0.0f });

				points[jointIndex - 1] = previousJoint + previousToCurrent;
			}

			// forwards
			points.front() = chain.front()->GetPosition();

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
					childTransform->PropagateParentLocalTransform(chain[jointIndex]->worldVecQuatMat);
				}

				Vec3F previousToCurrent = chain[jointIndex]->GetRotation() * Vec3F::Up() * lengths[jointIndex];
				DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + previousToCurrent);
				DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJoint + previousToCurrent, { 0.0f, 1.0f, 1.0f });
				DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJoint, { 0.0f, 1.0f, 1.0f });
				points[jointIndex + 1] = previousJoint + previousToCurrent;
			}
		}

		// std::shared_ptr<Mesh> mesh = gameObject->GetComponent<Renderer>()->GetMesh();
		// auto& boneInfoMap = mesh->GetBoneInfoMap();
		//
		// // get bind pose orientation of beginning of chain
		// QuatF accumulatedBindPose = boneInfoMap.at(chain[0]->GetGameObject().GetName()).offsetRotation.GetInverse();
		// for (int jointIndex = 0; jointIndex < points.size() - 1; ++jointIndex)
		// {
		// 	Vec3F previousJoint = points[jointIndex];
		// 	Vec3F currentJoint = points[jointIndex + 1];
		//
		// 	DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJoint, { 1.0f, 0.0f, 0.0f });
		// 	DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, currentJoint);
		//
		// 	Vec3F previousToCurrentDirection = (currentJoint - chain[jointIndex]->GetPosition()).Normalize();
		//
		// 	QuatF currentBindPoseOrientation = boneInfoMap.at(chain[jointIndex]->GetGameObject().GetName()).offsetRotation.GetInverse();
		// 	QuatF nextBindPoseOrientation = boneInfoMap.at(chain[jointIndex + 1]->GetGameObject().GetName()).offsetRotation.GetInverse();
		// 	QuatF offset = currentBindPoseOrientation.GetInverse() * nextBindPoseOrientation;
		// 	Vec3F dir = (accumulatedBindPose * offset) * Vec3F::Up();
		//
		//
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + previousToCurrentDirection * 2.0f);
		// 	DebugSystemLocator::GetDebugSystem()->DrawLine(chain[jointIndex]->GetPosition(), chain[jointIndex]->GetPosition() + dir * 1.0f);
		// 	DebugSystemLocator::GetDebugSystem()->DrawPoint(chain[jointIndex]->GetPosition(), {0.0f, 0.0f, 1.0f});
		//
		// 	QuatF toLocalFromBindPose = accumulatedBindPose.GetInverse();
		// 	QuatF rotation = QuatF::FromToRotation(toLocalFromBindPose * dir, toLocalFromBindPose * previousToCurrentDirection);
		//
		// 	// validate local bind pose is correct
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * localBindPoseOffset.GetInverse() * Vec3F::Up());
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * localBindPoseOffset.GetInverse() * Vec3F::Forward());
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * localBindPoseOffset.GetInverse() * Vec3F::Right());
		// 	//
		// 	// // validate rotation is correct
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * rotation * Vec3F::Up());
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * rotation * Vec3F::Forward());
		// 	// DebugSystemLocator::GetDebugSystem()->DrawLine(previousJoint, previousJoint + accumulatedBindPose * rotation * Vec3F::Right());
		// 	// DebugSystemLocator::GetDebugSystem()->DrawPoint(previousJoint + accumulatedBindPose * rotation * Vec3F::Up() * lengths[jointIndex], { 0.0f, 0.0f, 1.0f });
		// 	//
		// 	// if (jointIndex == 0)
		// 	// {
		// 	// 	Vec3F angles = rotation.GetEulerAngles();
		// 	// 	ENGINE_LOG_INFO("Arm Before: {0}\n", angles.ToString());
		// 	// 	angles.x = std::clamp(angles.x, -30.0f, 30.0f);
		// 	// 	angles.y = 0;// std::clamp(angles.y, -90.0f, 90.0f);
		// 	// 	angles.z = std::clamp(angles.z, -30.0f, 30.0f);
		// 	// 	ENGINE_LOG_INFO("Arm Angles of local: {0}\n", angles.ToString());
		// 	// 	rotation = QuatF::MakeRotationFromEulers(angles);
		// 	// }
		//
		// 	// if (jointIndex == 1)
		// 	// {
		// 	// 	Vec3F angles = rotation.GetEulerAngles();
		// 	// 	ENGINE_LOG_INFO("Arm Before: {0}\n", angles.ToString());
		// 	// 	angles.x = std::clamp(angles.x, -90.0f, 90.0f);
		// 	// 	angles.y = 0;// std::clamp(angles.y, -90.0f, 90.0f);
		// 	// 	angles.z = std::clamp(angles.z, -90.0f, 90.0f);
		// 	// 	ENGINE_LOG_INFO("Arm Angles of local: {0}\n", angles.ToString());
		// 	// 	rotation = QuatF::MakeRotationFromEulers(angles);
		// 	// }
		// 	//
		// 	// if (jointIndex == 2)
		// 	// {
		// 	// 	Vec3F angles = rotation.GetEulerAngles();
		// 	// 	ENGINE_LOG_INFO("Elbow Before: {0}\n", angles.ToString());
		// 	// 	angles.x = 0;// std::clamp(angles.x, 0.0f, 90.0f);
		// 	// 	angles.y = std::clamp(angles.y, 0.0f, 180.0f);
		// 	// 	angles.z = std::clamp(angles.z, 0.0f, 90.0f);
		// 	// 	ENGINE_LOG_INFO("Angles of local: {0}\n", angles.ToString());
		// 	// 	rotation = QuatF::MakeRotationFromEulers(angles);
		// 	// }
		//
		// 	QuatF parentBindPoseOrientation = boneInfoMap.at(chain[jointIndex]->GetParent().GetGameObject().GetName()).offsetRotation;
		// 	QuatF localBindPoseOffset = parentBindPoseOrientation * currentBindPoseOrientation;
		//
		// 	ENGINE_LOG_INFO("Local Bind Pose Offset {0}: {1}", jointIndex, localBindPoseOffset.GetEulerAngles().ToString());
		// 	ENGINE_LOG_INFO("Offset {0}: {1}", jointIndex, offset.GetEulerAngles().ToString());
		// 	ENGINE_LOG_INFO("Rotation angles for joing {0}: {1}\n", jointIndex, rotation.GetEulerAngles().ToString());
		//
		//
		// 	accumulatedBindPose = accumulatedBindPose * rotation;
		// 	if (jointIndex == 0)
		// 	{
		// 		chain[jointIndex]->SetLocalRotation(localBindPoseOffset * rotation);
		// 	}
		// 	else
		// 	{
		// 		chain[jointIndex]->SetLocalRotation(rotation);
		// 	}
		// 	
		// 	chain[jointIndex]->GetWorldMatrix();
		// 	for (int childIndex = 0; childIndex < chain[jointIndex]->GetNumberOfChildren(); childIndex++)
		// 	{
		// 		Transform* childTransform = chain[jointIndex]->GetChildAtIndex(childIndex);
		// 		childTransform->PropagateParentLocalTransform(chain[jointIndex]->worldVecQuatMat);
		// 	}
		// }
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
