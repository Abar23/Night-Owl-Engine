#include "CameraController.h"

#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

void CameraController::Start()
{
	mainCamera = NightOwl::Camera::GetMainCamera();
	mainCameraTransform = mainCamera->GetGameObject().GetTransform();
	SetCameraToDefaults();
}

void CameraController::Update()
{
	const float deltaTime = NightOwl::Time::GetDeltaTime();
	HandleForwardInput(deltaTime);
	HandleSideInput(deltaTime);
	HandleZoomInput(deltaTime);
	HandleRotationInput(deltaTime);
	HandleViewResetInput();
}

void CameraController::HandleForwardInput(const float deltaTime) const
{
	const NightOwl::Vec3F forwardVector = mainCameraTransform->GetForward();

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPad2))
	{
		mainCameraTransform->Translate(forwardVector * 10 * deltaTime, NightOwl::Space::World);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPad8))
	{
		mainCameraTransform->Translate(forwardVector * -10 * deltaTime, NightOwl::Space::World);
	}
}

void CameraController::HandleSideInput(const float deltaTime) const
{
	const NightOwl::Vec3F rightVector = mainCameraTransform->GetRight();

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPad4))
	{
		mainCameraTransform->Translate(rightVector * -10 * deltaTime, NightOwl::Space::World);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPad6))
	{
		mainCameraTransform->Translate(rightVector * 10 * deltaTime, NightOwl::Space::World);
	}
}

void CameraController::HandleZoomInput(const float deltaTime) const
{
	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPadAdd))
	{
		mainCamera->SetFieldOfView(-15.0f * deltaTime + mainCamera->FieldOfView());
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPadSubtract))
	{
		mainCamera->SetFieldOfView(15.0f * deltaTime + mainCamera->FieldOfView());
	}
}

void CameraController::HandleRotationInput(const float deltaTime) const
{
	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPad0))
	{
		mainCameraTransform->Rotate({ 0, 100 * deltaTime, 0 }, NightOwl::Space::World);
	}

	if (NightOwl::Input::IsKeyHeld(NightOwl::KeyCode::NumPadDecimal))
	{
		mainCameraTransform->Rotate({ 0, -100 * deltaTime, 0 }, NightOwl::Space::World);
	}
}

void CameraController::HandleViewResetInput() const
{
	if (NightOwl::Input::IsKeyPressed(NightOwl::KeyCode::NumPad5))
	{
		SetCameraToDefaults();
	}
}

void CameraController::SetCameraToDefaults() const
{
	//mainCameraTransform->SetPosition(defaultPosition);
	mainCameraTransform->SetRotation(NightOwl::QuatF());
	mainCamera->SetFieldOfView(defaultZoom);
}
