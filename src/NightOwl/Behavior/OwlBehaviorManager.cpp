#include "OwlBehaviorManager.h"
#include "OwlBehaviorState.h"
#include "OwlBehavior.h"

namespace NightOwl::Behavior
{
	OwlBehaviorManager::OwlBehaviorManager()
		:	hasInitializedBehaviors(false)
	{ }

	void OwlBehaviorManager::Update()
	{
		if(!owlBehaviorsToStart.empty())
		{
			for (auto* owlBehavior : owlBehaviorsToStart)
			{
				owlBehavior->Start();
				owlBehaviorsToUpdate.insert(owlBehavior);
				owlBehavior->state = OwlBehaviorState::Update;
			}
			owlBehaviorsToStart.clear();
		}

		for (auto* owlBehavior : owlBehaviorsToUpdate)
		{
			if(owlBehavior->state == OwlBehaviorState::Update)
			{
				if (owlBehavior->IsActiveAndEnabled())
				{
					owlBehavior->Update();
				}
				else if (!owlBehavior->isEnabled)
				{
					owlBehavior->OnDisable();
					owlBehavior->state = OwlBehaviorState::OnDisable;
				}
			}
			else if(owlBehavior->state == OwlBehaviorState::OnDisable && owlBehavior->isEnabled)
			{
				owlBehavior->OnEnable();
				owlBehavior->state = OwlBehaviorState::Update;
			}
		}
	}

	void OwlBehaviorManager::InitBehaviors()
	{
		if (!owlBehaviorsToAwake.empty())
		{
			for (auto* owlBehavior : owlBehaviorsToAwake)
			{
				owlBehavior->Awake();
				owlBehavior->OnEnable();
				owlBehaviorsToStart.insert(owlBehavior);
				owlBehavior->state = OwlBehaviorState::Start;
			}

			for (auto* owlBehavior : owlBehaviorsToStart)
			{
				owlBehavior->Start();
				owlBehaviorsToUpdate.insert(owlBehavior);
				owlBehavior->state = OwlBehaviorState::Update;
			}

			owlBehaviorsToAwake.clear();
			owlBehaviorsToStart.clear();
		}
	}

	void OwlBehaviorManager::AddOwlBehavior(OwlBehavior* owlBehavior)
	{
		owlBehaviors.insert(owlBehavior);

		if(!hasInitializedBehaviors)
		{
			owlBehaviorsToAwake.insert(owlBehavior);
			owlBehavior->state = OwlBehaviorState::Awake;
		}
		else
		{
			owlBehavior->Awake();
			owlBehavior->OnEnable();
			owlBehaviorsToStart.insert(owlBehavior);
			owlBehavior->state = OwlBehaviorState::Start;
		}
	}

	void OwlBehaviorManager::RemoveOwlBehavior(OwlBehavior* owlBehavior)
	{
		owlBehaviors.erase(owlBehavior);

		switch (owlBehavior->state)
		{
		case OwlBehaviorState::Awake:
			owlBehaviorsToAwake.erase(owlBehavior);

		case OwlBehaviorState::Update:
			owlBehaviorsToUpdate.erase(owlBehavior);

		default:
			owlBehaviorsToStart.erase(owlBehavior);
		}
	}

	void OwlBehaviorManager::Reset()
	{
		owlBehaviors.clear();
		owlBehaviorsToUpdate.clear();
		owlBehaviorsToUpdate.clear();
		owlBehaviorsToAwake.clear();
		hasInitializedBehaviors = false;
	}
}
