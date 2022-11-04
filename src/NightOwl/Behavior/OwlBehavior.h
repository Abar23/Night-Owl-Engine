#pragma once

#include "OwlBehaviorState.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Physics/2D/Structures/Collision2D.h"

namespace NightOwl::Behavior
{
	class OwlBehavior : public Component::Component
	{
	public:
		OwlBehavior()
			:	Component(NightOwl::Component::ComponentType::OwlBehavior), state(OwlBehaviorState::Awake)
		{ }

		virtual void Awake() { }

		virtual void Start() { }

		virtual void Update() { }

		virtual void OnEnable() { }

		virtual void OnDisable() { }

		virtual void OnPreRender() { }

		virtual void OnRender() { }

		virtual void OnPostRender() { }

		virtual void OnCollisionEnter2D(const Physics::Collision2D& collision2D) { }

		virtual void OnCollisionStay2D(const Physics::Collision2D& collision2D) { }

		virtual void OnCollisionExit2D(const Physics::Collision2D& collision2D) { }

		bool IsActiveAndEnabled() const;

	private:
		OwlBehaviorState state;

		friend class OwlBehaviorManager;
	};
}
