#include "Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl::Component
{
	Camera* Camera::mainCamera{ nullptr };

	Camera::Camera()
		:	Component(ComponentType::Camera),
			fieldOfView(90.0f),
			clippingPlanes(0.01f, 1000.0f),
			isPerspectiveProjection(true),
			orthographicSize(1.0f),
			isProjectionDirty(false)
	{
		this->gameObject = gameObject;
		projectionMatrix = Math::Mat4F::Perspective(fieldOfView, Window::WindowApi::GetWindow()->GetAspectRatio(), clippingPlanes.near, clippingPlanes.far);
		mainCamera = this;
	}

	void Camera::LookAt(Math::Vec3F pointToLookAt)
	{
		Math::Mat4F::LookAt(gameObject->GetTransform()->GetPosition(), pointToLookAt, Math::Vec3F::Up());
	}

	Math::Mat4F Camera::ViewProjectionMatrix()
	{

		if (isPerspectiveProjection)
		{
			projectionMatrix = Math::Mat4F::Perspective(fieldOfView, Window::WindowApi::GetWindow()->GetAspectRatio(), clippingPlanes.near, clippingPlanes.far);
		}
		else
		{
			const float height = 2.0f * orthographicSize;
			const float width = height * Window::WindowApi::GetWindow()->GetAspectRatio();
			const float halfHeight = height / 2.0f;
			const float halfWidth = width / 2.0f;
			projectionMatrix = Math::Mat4F::Orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, clippingPlanes.near, clippingPlanes.far);
		}

		return projectionMatrix * GetViewMatrix();
	}

	const Math::Mat4F& Camera::GetViewMatrix()
	{
		return gameObject->GetTransform()->GetWorldMatrix().GetInverse();
	}

	const Math::Mat4F& Camera::GetViewMatrix() const
	{
		return gameObject->GetTransform()->GetWorldMatrix();
	}

	float Camera::GetNearClippingPlane()
	{
		return clippingPlanes.near;
	}

	void Camera::SetNearClippingPlane(float nearPlaneDistance)
	{
		clippingPlanes.near = nearPlaneDistance;
	}

	float Camera::GetFarClippingPlane()
	{
		return clippingPlanes.far;
	}

	void Camera::SetFarClippingPlane(float farPlaneDistance)
	{
		clippingPlanes.far = farPlaneDistance;
	}

	ClippingPlanes Camera::GetClippingPlanes()
	{
		return clippingPlanes;
	}

	void Camera::SetClippingPlanes(const ClippingPlanes& clippingPlanes)
	{
		this->clippingPlanes = clippingPlanes;
	}

	float Camera::FieldOfView() const
	{
		return fieldOfView;
	}

	void Camera::SetFieldOfView(float fieldOfView)
	{
		this->fieldOfView = fieldOfView;
	}

	void Camera::SetProjectionDirtyFlag()
	{
		isProjectionDirty = true;
	}

	float Camera::GetOrthographicSize() const
	{
		return orthographicSize;
	}

	void Camera::SetOrthographicSize(float orthographicSize)
	{
		this->orthographicSize = orthographicSize;
		SetProjectionDirtyFlag();
	}

	bool Camera::IsPerspectiveProjection()
	{
		return isPerspectiveProjection;
	}

	void Camera::SetPerspectiveMode(bool enablePerspectiveProjection)
	{
		this->isPerspectiveProjection = enablePerspectiveProjection;
	}

	Camera* Camera::GetMainCamera()
	{
		return mainCamera;
	}
}
