#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/CatmullRomSpline.h"
#include "NightOwl/Component/Concrete/Transform.h"

class SplineDebugger : public NightOwl::OwlBehavior
{
public:
	void Awake() override;

	void Update() override;

private:
	float arcLength;

	float t;

	NightOwl::Transform* transform;

	NightOwl::CatmullRomSpline* splineComponent;
};

