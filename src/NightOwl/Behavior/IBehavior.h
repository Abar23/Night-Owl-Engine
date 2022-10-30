#pragma once

namespace NightOwl::Behavior
{
	class IBehavior
	{
	public:
		virtual ~IBehavior() = default;

		virtual void Awake() = 0;

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void Enable() = 0;

		virtual void Disable() = 0;
	};
}
