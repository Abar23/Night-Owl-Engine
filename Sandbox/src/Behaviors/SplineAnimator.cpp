#include "SplineAnimator.h"

#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"

void SplineAnimator::Awake()
{
	t = 0.0f;
	previousT = 0.0f;
	shouldPlay = true;

	splineComponent = gameObject->GetComponent<NightOwl::CatmullRomSpline>();
	transform = gameObject->GetTransform();
	animator = gameObject->GetComponent<NightOwl::Animator>();
}

void SplineAnimator::Start()
{
	constexpr int numPoints = 10;
	for (int i = 0; i <= numPoints; ++i) 
	{
		const float angleRadians = NightOwl::DegreesToRad(i * (360.0f / numPoints));

		const float x = std::cos(angleRadians) * 10.f;
		const float z = std::sin(angleRadians) * 10.f;

		splineComponent->AddControlPoint(NightOwl::Vec3F(x, 0.1f, z));
	}

	splineComponent->ShouldLoop(true);
}

void SplineAnimator::Update()
{
	if (shouldPlay == false)
	{
		return;
	}

	previousPoint = transform->GetPosition();
	previousT = t;
	t += NightOwl::Time::GetDeltaTime();
	if (t > 11.0f)
	{
		previousT = 0.0f;
		t = NightOwl::Time::GetDeltaTime();
	}

	if (t > 10.0f)
	{
		return;
	}

	const float s = NightOwl::SinEaseInEaseOutWithConstantVelocity(t, 10.0f, 3.0f, 6.0f);

	const NightOwl::Vec3F currentPoint = splineComponent->EvaluateUsingArcLength(s);
	transform->SetPosition(currentPoint);

	const NightOwl::Vec3F tangent = splineComponent->EvaluateTangentUsingArcLength(s);
	transform->LookAt(transform->GetPosition() + tangent);

	const NightOwl::Vec3F velocity = (transform->GetPosition() - previousPoint) / (t - previousT);
	animator->SetFloat("velocity", velocity.Magnitude() * 0.1f);
}

void SplineAnimator::Reset()
{
	t = 0.0f;
	transform->SetPosition(splineComponent->EvaluateUsingParameter(0));
}

void SplineAnimator::Play()
{
	shouldPlay = true;
}

void SplineAnimator::Pause()
{
	shouldPlay = false;
}
