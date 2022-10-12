#pragma once

#include "NightOwl/Behavior/IBehavior.h"
#include "NightOwl/Component/Component.h"

namespace NightOwl::Behavior
{
	class IOwlBehavior : public IBehavior, public Component::Component
	{
	public:
	};
}
