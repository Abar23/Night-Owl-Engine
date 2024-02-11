#include <NightOwlPch.h>

#include "OwlBehavior.h"
#include "OwlBehaviorManager.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	bool OwlBehavior::IsActiveAndEnabled() const
	{
		return gameObject->IsActive() && this->isEnabled;
	}

	void OwlBehavior::Remove()
	{
		OwlBehaviorManagerLocator::Get()->RemoveOwlBehavior(this);
	}
}
