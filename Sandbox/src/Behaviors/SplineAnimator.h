#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/CatmullRomSpline.h"
#include "NightOwl/Component/Concrete/Transform.h"

class SplineAnimator : public NightOwl::OwlBehavior
{
public:
	void Awake() override;

	void Start() override;

	void Update() override;

	void LateUpdate() override;

	void Reset();

private:
	float speedFactor;

	float maxSpeed;

	float distanceToMinSpeed;

	float distanceToMaxSpeed;

	float chainIkWeightSpeed;

	bool wasReset;

	NightOwl::CatmullRomSpline* splineComponent;

	NightOwl::Transform* transform;

	NightOwl::Animator* animator;

	NightOwl::ChainIK* chain;

	NightOwl::Transform* target;

	NightOwl::Vec3F playerPosition;

	NightOwl::Vec3F targetPosition;

	void UpdatePath();

	void MoveCharacter();

	void AnimateChainToTarget();
};

