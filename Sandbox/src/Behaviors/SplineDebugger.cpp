#include "SplineDebugger.h"

#include "NightOwl/Core/Locator/DebugSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"

void SplineDebugger::Awake()
{
	transform = gameObject->GetTransform();
	splineComponent = gameObject->GetComponent<NightOwl::CatmullRomSpline>();
	animator = gameObject->GetComponent<NightOwl::Animator>();
	arcLength = 0.0f;
	t = 0.0f;
	previousT = 0.0f;
}

void SplineDebugger::Update()
{
	NightOwl::DebugSystem* debug = NightOwl::DebugSystemLocator::GetDebugSystem();

	NightOwl::Vec3F	previousPoint = splineComponent->EvaluateUsingParameter(0.0f);

	// Draw path
	float u = 0.0f;
	const float numberOfSegments = static_cast<float>(splineComponent->GetNumberOfCurveSegments());
	const float step = 0.01f;
	while (u < numberOfSegments)
	{
		u += step;
		NightOwl::Vec3F nextPoint = splineComponent->EvaluateUsingParameter(u);
		debug->DrawLine(previousPoint, nextPoint);

		previousPoint = nextPoint;
	}

	// Draw points
	for (int controlPointIndex = 0; controlPointIndex < splineComponent->GetNumberOfControlPoints(); ++controlPointIndex)
	{
		debug->DrawPoint(splineComponent->GetControlPointAtIndex(controlPointIndex), { 1.0f, 0.0f, 0.0f });
	}

	transform->SetLocalPosition(splineComponent->EvaluateUsingArcLength(arcLength));

	NightOwl::Vec3F tangent = splineComponent->EvaluateTangentUsingArcLength(arcLength);

	debug->DrawLine(transform->GetPosition(), transform->GetPosition() + tangent);

	transform->LookAt(transform->GetPosition() + tangent);

	previousT = t;
	t += NightOwl::Time::GetDeltaTime();
	if (t > 10.0f)
	{
		previousT = 0.0f;
		t = NightOwl::Time::GetDeltaTime();
	}

	float previousArcLength = NightOwl::SinEaseInEaseOutWithConstantVelocity(previousT, 10.0f, 3.0f, 7.0f);
	arcLength = NightOwl::SinEaseInEaseOutWithConstantVelocity(t, 10.0f, 3.0f, 7.0f);

	float velocity = (arcLength - previousArcLength) / (t - previousT);

	animator->SetFloat("test", velocity * 7.5f);
}
