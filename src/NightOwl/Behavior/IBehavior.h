#pragma once

namespace NightOwl::Behavior
{
	class IBehavior
	{
	public:
		virtual ~IBehavior() = default;

		virtual void OnAwake() = 0;

		virtual void OnUpdate() = 0;

		virtual void OnEnable() = 0;

		virtual void OnDisable() = 0;
	};
}
