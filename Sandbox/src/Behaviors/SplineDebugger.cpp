#include "SplineDebugger.h"

#include "NightOwl/Core/Locator/DebugSystemLocator.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"

void SplineDebugger::Awake()
{
	transform = gameObject->GetTransform();
	splineComponent = gameObject->GetComponent<NightOwl::CatmullRomSpline>();
	arcLength = 0.0f;
	t = 0.0f;
}

void SplineDebugger::Update()
{
	NightOwl::DebugSystem* debug = NightOwl::DebugSystemLocator::GetDebugSystem();

	NightOwl::Vec3F	previousPoint = splineComponent->EvaluateUsingParameter(0.0f);

	// Draw path
	float u = 0.0f;
	float numberOfSegments = static_cast<float>(splineComponent->GetNumberOfCurveSegments());
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

	NightOwl::Vec3F targetPoint = splineComponent->EvaluateUsingArcLength(arcLength + 0.01f);

	transform->LookAt(targetPoint);

	t += NightOwl::Time::GetDeltaTime() * 0.01f;

	arcLength = -(std::cosf(NightOwl::F_PI * t) - 1.0f) / 2.0f;
	//arcLength = t < 0.5 ? 16.0f * t * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;

	if (t > 1.0f)
	{
		arcLength = 0.0f;
		t = 0.0f;
	}
}
