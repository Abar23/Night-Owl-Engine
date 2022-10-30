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

private:
	NightOwl::Component::RigidBody2D* rigidBody2D;
};
