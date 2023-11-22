#pragma once

#include <unordered_set>

namespace NightOwl
{
	class OwlBehavior;

	class OwlBehaviorManager
	{
	public:
		OwlBehaviorManager();

		void Update();

		void FixedUpdate();

		void LateUpdate();

		void InitBehaviors();

		void AddOwlBehavior(OwlBehavior* owlBehavior);

		void RemoveOwlBehavior(OwlBehavior* owlBehavior);

		void Reset();

	private:
		// TODO: May not need the below set
		std::unordered_set<OwlBehavior*> owlBehaviors;

		std::unordered_set<OwlBehavior*> owlBehaviorsToStart;

		std::unordered_set<OwlBehavior*> owlBehaviorsToUpdate;

		std::unordered_set<OwlBehavior*> owlBehaviorsToAwake;

		bool hasInitializedBehaviors;

		void AwakeAndEnableOwlBehaviors();

		void StartOwlBehaviors();

		void UpdateOwlBehaviors();
	};
}
