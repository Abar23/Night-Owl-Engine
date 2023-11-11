#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Transform.h"

class IkTargetController : public NightOwl::OwlBehavior
{
public:
	void Start() override;

	void Update() override;

	void SetSpeed(float speed);

private:
	float speed;

	NightOwl::Transform* transform;
};
