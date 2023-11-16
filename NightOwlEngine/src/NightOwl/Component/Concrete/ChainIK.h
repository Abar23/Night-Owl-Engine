#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Math/Vec3.h"
#include "NightOwl/Math/Quaternion.h"
#include <vector>

namespace NightOwl
{
	class Transform;

	class ChainIK : public Component
	{
	public:
		ChainIK();

		void Update();

		void AddJointToChain(Transform* joint);

		void RemoveBackJoint();

		float GetWeight();

		void SetWeight(float weight);

		void SetTarget(Transform* target);

	private:
		float weight;

		float totalChainLength;

		Transform* target;

		inline static constexpr int MAX_ITERATIONS = 15;

		std::vector<Transform*> chain;

		void FabrikSolver();

		void Forward(std::vector<Vec3F>& points, std::vector<float>& lengths);

		void Backward(std::vector<Vec3F>& points, std::vector<float>& lengths);

		Vec3F Constrain(const Vec3F& previousPoint, const Vec3F& currentPoint, const Vec3F& targetPoint, const QuatF& currentJointOrientation, float jointLength, bool isBack = false);

		static float EvaluateEllipse(float x, float y, float a, float b);

		Vec2F FindNearestPointOnEllipsoid(float x, float y, float a, float b) const;

		void Remove() override;
	};
}
