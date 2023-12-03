#include "SplineAnimator.h"

#include "NightOwl/Component/Concrete/ChainIK.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"

void SplineAnimator::Awake()
{
	speedFactor = 0.25f;
	distanceToMinSpeed = 1.0f;
	distanceToMaxSpeed = 3.0f;
	maxSpeed = 1.0f;
	chainIkWeightSpeed = 1.25f;
	wasReset = false;

	splineComponent = gameObject->GetComponent<NightOwl::CatmullRomSpline>();
	transform = gameObject->GetTransform();
	animator = gameObject->GetComponent<NightOwl::Animator>();
	chain = gameObject->GetComponent<NightOwl::ChainIK>();
	target = gameObject->GetScene()->FindWithName("Target")->GetTransform();
}

void SplineAnimator::Start()
{
	const NightOwl::Vec3F playerPosition = transform->GetPosition();
	const NightOwl::Vec3F targetPosition = { target->GetPosition().x, 0, target->GetPosition().z };

	splineComponent->AddControlPoint({ playerPosition.x, 0, playerPosition.z });
	splineComponent->AddControlPoint(targetPosition);

	chain->SetWeight(0.0f);
}

void SplineAnimator::Update()
{
	UpdatePath();

	AnimateChainToTarget();

	MoveCharacter();

	if (wasReset)
	{
		chain->ForceSolve();
		wasReset = false;
	}
}

void SplineAnimator::LateUpdate()
{
	chain->ApplySolveToChain();
}

void SplineAnimator::Reset()
{
	transform->SetPosition(NightOwl::Vec3F::Zero());
	chain->SetWeight(0);
	wasReset = true;
}

void SplineAnimator::UpdatePath()
{
	playerPosition = transform->GetPosition();
	targetPosition = { target->GetPosition().x, 0, target->GetPosition().z };

	splineComponent->UpdateControlPointAtIndex(0, { playerPosition.x, 0, playerPosition.z });
	splineComponent->UpdateControlPointAtIndex(1, targetPosition);
}

void SplineAnimator::MoveCharacter()
{
	float distanceFromPlayerToTarget = splineComponent->GetArcLength();
	float speed = maxSpeed;
	if (distanceFromPlayerToTarget < distanceToMaxSpeed)
	{
		speed = NightOwl::MapValueInRange(distanceFromPlayerToTarget, distanceToMinSpeed, distanceToMaxSpeed, 0.0f, maxSpeed);
	}

	float distanceToTravelAlongSpline = speed * NightOwl::Time::GetDeltaTime();

	const NightOwl::Vec3F currentPoint = splineComponent->EvaluateUsingArcLength(distanceToTravelAlongSpline);
	transform->SetPosition(currentPoint);

	const NightOwl::Vec3F tangent = splineComponent->EvaluateTangentUsingArcLength(distanceToTravelAlongSpline);
	transform->LookAt(transform->GetPosition() + tangent);

	animator->SetFloat("velocity", speed * speedFactor);
}

void SplineAnimator::AnimateChainToTarget()
{
	const float distanceFromPlayerToTarget = splineComponent->GetArcLength();
	if (distanceFromPlayerToTarget < 1.4f)
	{
		float currentVelocity = animator->GetFloat("velocity");
		if (currentVelocity > NightOwl::EPSILON)
		{
			currentVelocity *= 0.5f;
			animator->SetFloat("velocity", currentVelocity);
		}

		const float currentWeight = chain->GetWeight();
		const float newWeight = NightOwl::Lerp(currentWeight, 1.0f, chainIkWeightSpeed * NightOwl::Time::GetDeltaTime());
		chain->SetWeight(newWeight);
		return;
	}

	const float currentWeight = chain->GetWeight();
	const float newWeight = NightOwl::Lerp(currentWeight, 0.0f, chainIkWeightSpeed * NightOwl::Time::GetDeltaTime());
	chain->SetWeight(newWeight);
}
