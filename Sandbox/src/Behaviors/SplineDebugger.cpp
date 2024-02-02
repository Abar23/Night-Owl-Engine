#include "SplineDebugger.h"

#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Debugging/DebugSystem.h"

void SplineDebugger::Awake()
{
	transform = gameObject->GetTransform();
	splineComponent = gameObject->GetComponent<NightOwl::CatmullRomSpline>();
}

void SplineDebugger::Update()
{
	NightOwl::DebugSystem* debug = NightOwl::DebugSystemLocator::Get();

	NightOwl::Vec3F	previousPoint = splineComponent->EvaluateUsingParameter(0.0f);

	// Debug draw spline
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

	// Debug draw control points
	for (int controlPointIndex = 0; controlPointIndex < splineComponent->GetNumberOfControlPoints(); ++controlPointIndex)
	{
		debug->DrawPoint(splineComponent->GetControlPointAtIndex(controlPointIndex), { 1.0f, 0.0f, 0.0f });
	}
}
