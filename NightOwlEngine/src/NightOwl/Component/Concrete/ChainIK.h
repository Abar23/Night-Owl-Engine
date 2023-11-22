#pragma once

#include <map>

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec3.h"
#include "NightOwl/Math/Quaternion.h"
#include <vector>

#include "NightOwl/Animation/3D/Structures/JointConstraints.h"

namespace NightOwl
{
	class BoneInfo;

	class Mesh;

	class Transform;

	class ChainIK : public Component
	{
	public:
		ChainIK();

		void Start() override;

		void Update();

		std::shared_ptr<Component> Clone() override;

		void SetBallAndSocketConstraintForJoint(int jointIndex, const BallAndSocketConstraint& constraints);

		void AddJointToChain(Transform* joint);

		void RemoveBackJoint();

		float GetWeight() const;

		void SetWeight(float weight);

		void SetTarget(Transform* target);

		void ApplySolveToChain();

		void EnableConstraints(bool enable);

		bool AreConstraintsEnabled() const;

		void ForceSolve();

	private:
		float weight;

		float totalChainLength;

		bool shouldApplyJointConstraints;

		const BoneInfo* parentBoneInfo;

		Vec3F previousTargetPosition;

		Vec3F previousGameObjectPosition;

		Transform* target;

		std::shared_ptr<Mesh> mesh;

		std::vector<Transform*> chain;

		std::vector<const BoneInfo*> bindPoses;

		std::vector<float> chainLengths;

		std::vector<Vec3F> fabrikPoints;

		std::map<size_t, BallAndSocketConstraint> jointBallAndSocketMap;

		void FabrikSolver();

		void Forward();

		void Backward();

		Vec3F ConstrainBallAndSocket(const Vec3F& previousPoint, const Vec3F& currentPoint, const Vec3F& targetPoint, const QuatF& currentJointOrientation, float jointLength, int jointIndex) const;

		static float EvaluateEllipse(float x, float y, float a, float b);

		Vec2F FindNearestPointOnEllipsoid(float x, float y, float a, float b) const;

		void Remove() override;
	};
}
