#include <NightOwlPch.h>

#include "OwlBehaviorManager.h"
#include "OwlBehaviorState.h"
#include "OwlBehavior.h"
#include "NightOwl/GameObject/GameObject.h"


namespace NightOwl
{
	OwlBehaviorManager::OwlBehaviorManager()
		:	hasInitializedBehaviors(false)
	{ }

	void OwlBehaviorManager::Update()
	{
		StartOwlBehaviors();
		AwakeAndEnableOwlBehaviors();
		UpdateOwlBehaviors();
	}

	void OwlBehaviorManager::InitBehaviors()
	{
		AwakeAndEnableOwlBehaviors();
		StartOwlBehaviors();
	}

	void OwlBehaviorManager::AddOwlBehavior(OwlBehavior* owlBehavior)
	{
		owlBehaviors.insert(owlBehavior);

		if(!hasInitializedBehaviors)
		{
			owlBehaviorsToAwake.insert(owlBehavior);
			owlBehavior->state = OwlBehaviorState::Awake;
			return;
		}

		owlBehavior->Awake();
		owlBehavior->OnEnable();
		owlBehaviorsToStart.insert(owlBehavior);
		owlBehavior->state = OwlBehaviorState::Start;
	}

	void OwlBehaviorManager::RemoveOwlBehavior(OwlBehavior* owlBehavior)
	{
		owlBehaviors.erase(owlBehavior);
		owlBehaviorsToAwake.erase(owlBehavior);
		owlBehaviorsToUpdate.erase(owlBehavior);
		owlBehaviorsToStart.erase(owlBehavior);
	}

	void OwlBehaviorManager::Reset()
	{
		owlBehaviors.clear();
		owlBehaviorsToUpdate.clear();
		owlBehaviorsToUpdate.clear();
		owlBehaviorsToAwake.clear();
		hasInitializedBehaviors = false;
	}

	void OwlBehaviorManager::AwakeAndEnableOwlBehaviors()
	{
		for (auto it = owlBehaviorsToAwake.begin(); it != owlBehaviorsToAwake.end();)
		{
			OwlBehavior* owlBehavior = *it;

			if(!owlBehavior->IsActiveAndEnabled())
			{
				++it;
				continue;
			}

			owlBehavior->Awake();
			owlBehavior->OnEnable();
			owlBehaviorsToStart.insert(owlBehavior);
			owlBehavior->state = OwlBehaviorState::Start;
			it = owlBehaviorsToAwake.erase(it);
		}
	}

	void OwlBehaviorManager::StartOwlBehaviors()
	{
		for (auto it = owlBehaviorsToStart.begin(); it != owlBehaviorsToStart.end();)
		{
			OwlBehavior* owlBehavior = *it;

			if(!owlBehavior->IsActiveAndEnabled())
			{
				++it;
				continue;
			}

			owlBehavior->Start();
			owlBehaviorsToUpdate.insert(owlBehavior);
			owlBehavior->state = OwlBehaviorState::Update;
			it = owlBehaviorsToStart.erase(it);
		}
	}

	void OwlBehaviorManager::UpdateOwlBehaviors()
	{
		for (auto owlBehavior : owlBehaviorsToUpdate)
		{
			if (owlBehavior->state == OwlBehaviorState::Update)
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
			else if (owlBehavior->state == OwlBehaviorState::OnDisable && owlBehavior->isEnabled)
			{
				owlBehavior->OnEnable();
				owlBehavior->state = OwlBehaviorState::Update;
			}
		}
	}

	void OwlBehaviorManager::FixedUpdate()
	{
		for (auto& behaviorsToUpdate : owlBehaviorsToUpdate)
		{
			behaviorsToUpdate->FixedUpdate();
		}
	}
}
