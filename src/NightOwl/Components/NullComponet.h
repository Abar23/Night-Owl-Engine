#pragma once

#include "Component.h"

namespace NightOwl::Components
{
	class NullComponent : public Component
	{
		NullComponent()
			: Component(nullptr, ComponentType::None)
		{
			
		}
	};
}
