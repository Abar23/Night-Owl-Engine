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

	void Reset();

	void Play();

	void Pause();

private:
	NightOwl::Vec3F previousPoint;

	float t;

	float previousT;

	bool shouldPlay;

	NightOwl::CatmullRomSpline* splineComponent;

	NightOwl::Transform* transform;

	NightOwl::Animator* animator;
};

