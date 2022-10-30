#pragma once

#include <set>

namespace NightOwl::Behavior
{
	class OwlBehavior;

	class OwlBehaviorManager
	{
	public:
		OwlBehaviorManager();

		void Update();

		void InitBehaviors();

		void AddOwlBehavior(OwlBehavior* owlBehavior);

		void RemoveOwlBehavior(OwlBehavior* owlBehavior);

		void Reset();

	private:
		std::set<OwlBehavior*> owlBehaviors;

		std::set<OwlBehavior*> owlBehaviorsToStart;

		std::set<OwlBehavior*> owlBehaviorsToUpdate;

		std::set<OwlBehavior*> owlBehaviorsToAwake;

		bool hasInitializedBehaviors;
	};
}
