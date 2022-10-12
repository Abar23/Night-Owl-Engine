#pragma once

#include "IOwlBehavior.h"

namespace NightOwl::Behavior
{
	class CustomBehavior : public IOwlBehavior 
	{
	public:
		void OnAwake() override;
		void OnUpdate() override;
		void OnEnable() override;
		void OnDisable() override;
	};
}
