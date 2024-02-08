#pragma once

#include "NightOwl/Behavior/OwlBehavior.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Transform.h"

class CameraController : public NightOwl::OwlBehavior
{
public:
	void Start() override;

	void Update() override;

private:
	NightOwl::Camera* mainCamera = nullptr;

	NightOwl::Transform* mainCameraTransform = nullptr;

	inline static NightOwl::Vec3F defaultPosition = { 0.0f, 0.5f, 0.0f };

	inline static float defaultZoom = 70.0f;

	void HandleForwardInput(const float deltaTime) const;

	void HandleSideInput(const float deltaTime) const;

	void HandleZoomInput(const float deltaTime) const;

	void HandleRotationInput(const float deltaTime) const;

	void HandleViewResetInput() const;

	void SetCameraToDefaults() const;
};
