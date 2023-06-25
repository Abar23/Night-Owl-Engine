#include <NightOwlPch.h>

#include "OwlBehavior.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Core/Locator/OwlBehaviorManagerLocator.h"

namespace NightOwl
{
	bool OwlBehavior::IsActiveAndEnabled() const
	{
		return gameObject->IsActive() && this->isEnabled;
	}

	void OwlBehavior::Remove()
	{
		OwlBehaviorManagerLocator::GetOwlBehaviorManager()->RemoveOwlBehavior(this);
	}
}
