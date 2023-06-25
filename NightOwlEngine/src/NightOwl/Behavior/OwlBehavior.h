#pragma once

#include "OwlBehaviorState.h"
#include "NightOwl/Component/Component.h"
#include "NightOwl/Physics/2D/Structures/Collision2D.h"

namespace NightOwl
{
	class OwlBehavior : public Component
	{
	public:
		OwlBehavior()
			:	Component(ComponentType::OwlBehavior),
				state(OwlBehaviorState::Awake)
		{ }

		virtual void Awake() { }

		virtual void Start() { }

		virtual void Update() { }

		virtual void FixedUpdate() { }

		virtual void OnEnable() { }

		virtual void OnDisable() { }

		//virtual void OnPreRender() { }

		//virtual void OnRender() { }

		//virtual void OnPostRender() { }

		virtual void OnCollisionEnter2D(const Collision2D& collision2D) { }

		virtual void OnCollisionStay2D(const Collision2D& collision2D) { }

		virtual void OnCollisionExit2D(const Collision2D& collision2D) { }

		bool IsActiveAndEnabled() const;

	private:
		OwlBehaviorState state;

		friend class OwlBehaviorManager;

		void Remove() override;
	};
}
