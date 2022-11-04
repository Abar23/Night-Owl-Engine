#pragma once

#include "OwlBehavior.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"

class OwlBehaviorTest : public NightOwl::Behavior::OwlBehavior 
{
public:

	void Awake() override;

	void Start() override;

	void Update() override;

	void OnEnable() override;

	void OnDisable() override;

	void OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D) override;

	void OnCollisionStay2D(const NightOwl::Physics::Collision2D& collision2D) override;

	void OnCollisionExit2D(const NightOwl::Physics::Collision2D& collision2D) override;
private:
	NightOwl::Component::RigidBody2D* rigidBody2D;
};
