#include "OwlBehavior.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl::Behavior
{
	bool OwlBehavior::IsActiveAndEnabled() const
	{
		return gameObject->IsActive() && this->isEnabled;
	}
}
