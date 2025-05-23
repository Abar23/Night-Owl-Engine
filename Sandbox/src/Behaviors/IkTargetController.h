#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Transform.h"

class IkTargetController : public NightOwl::OwlBehavior
{
public:
	void Start() override;

	void Update() override;

	void SetSpeed(float speed);

	void Reset();

private:
	float speed;

	NightOwl::Vec3F originalLocation;

	NightOwl::Transform* transform;
};
