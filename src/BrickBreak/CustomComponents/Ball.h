#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"

namespace BrickBreak
{
	class Ball : public NightOwl::Behavior::OwlBehavior
	{
	public:
		void Awake() override;

		void Start() override;

		void Update() override;

		void OnEnable() override;

		void OnDisable() override;

		void OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D) override;

	private:
		NightOwl::Component::RigidBody2D* rigidBody;
	};
}
