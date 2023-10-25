#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Animator.h"
#include "NightOwl/Component/Concrete/CatmullRomSpline.h"
#include "NightOwl/Component/Concrete/Transform.h"

class SplineDebugger : public NightOwl::OwlBehavior
{
public:
	void Awake() override;

	void Update() override;

private:
	NightOwl::Transform* transform;

	NightOwl::CatmullRomSpline* splineComponent;
};

