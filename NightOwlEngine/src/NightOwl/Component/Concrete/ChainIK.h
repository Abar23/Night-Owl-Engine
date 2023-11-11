#pragma once

#include "NightOwl/Component/Component.h"
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

		void Forward();

		void Backwards();

		void Remove() override;
	};
}
