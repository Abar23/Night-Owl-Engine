#include "NightOwlPch.h"

#include "Camera.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Window/WindowApi.h"

namespace NightOwl
{
	Camera* Camera::mainCamera{ nullptr };

	Camera::Camera()
		:	Component(ComponentType::Camera),
			fieldOfView(90.0f),
			clippingPlanes(0.01f, 1000.0f),
			aspectRatio(WindowApi::GetWindow()->GetAspectRatio()),
			orthographicSize(WindowApi::GetWindow()->GetHeight() / 2.0f),
			isPerspectiveProjection(true),
			wasAspectRatioSet(false),
			isProjectionDirty(true)
	{
		this->gameObject = gameObject;
		mainCamera = this;
	}

	std::shared_ptr<Component> Camera::Clone()
	{
		std::shared_ptr<Camera> clone = std::make_shared<Camera>();

		clone->projectionMatrix = projectionMatrix;
		clone->fieldOfView = fieldOfView;
		clone->clippingPlanes = clippingPlanes;
		clone->isPerspectiveProjection = isPerspectiveProjection;
		clone->orthographicSize = orthographicSize;
		clone->isProjectionDirty = isProjectionDirty;

		// TODO: create copy and = operator for render texture

		return clone;
	}

	Mat4F Camera::GetViewProjectionMatrix()
	{
		auto* window = WindowApi::GetWindow().get();

		// This optimization will only work with one camera. Must add a collection of cameras
		// in the camera component to have the window indicate to each camera that the
		// aspect ratio has changed. Will work for single camera games.
		if(!isProjectionDirty && !window->HasWindowChangedAspectRatio())
		{
			return projectionMatrix * GetViewMatrix();
		}

		const float aspect = (wasAspectRatioSet) ? aspectRatio : window->GetAspectRatio();

		if (isPerspectiveProjection)
		{
			projectionMatrix = Mat4F::Perspective(fieldOfView, aspect, clippingPlanes.near, clippingPlanes.far);
		}
		else
		{
			const float virtualHeight = 2.0f * orthographicSize;
			const float virtualWidth = virtualHeight * aspect;

			const float windowAspectRatio = window->GetAspectRatio();

			// Have to map physical screen space to the virtual space of the orthographic camera using the window aspect ratio
			const float height = std::max(virtualWidth / windowAspectRatio, virtualHeight);
			const float width = std::max(windowAspectRatio * virtualHeight, virtualWidth);

			const float halfHeight = height / 2.0f;
			const float halfWidth = width / 2.0f;

			projectionMatrix = Mat4F::Orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, clippingPlanes.near, clippingPlanes.far);
		}

		isProjectionDirty = false;

		return projectionMatrix * GetViewMatrix();
	}

	Mat4F Camera::GetViewMatrix() const
	{
		return gameObject->GetTransform()->GetWorldMatrix().GetInverse();
	}

	float Camera::GetNearClippingPlane() const
	{
		return clippingPlanes.near;
	}

	void Camera::SetNearClippingPlane(float nearPlaneDistance)
	{
		clippingPlanes.near = nearPlaneDistance;
		SetProjectionDirtyFlag();
	}

	float Camera::GetFarClippingPlane() const
	{
		return clippingPlanes.far;
	}

	void Camera::SetFarClippingPlane(float farPlaneDistance)
	{
		clippingPlanes.far = farPlaneDistance;
		SetProjectionDirtyFlag();
	}

	ClippingPlanes Camera::GetClippingPlanes()
	{
		return clippingPlanes;
	}

	void Camera::SetClippingPlanes(const ClippingPlanes& clippingPlanes)
	{
		this->clippingPlanes = clippingPlanes;
		SetProjectionDirtyFlag();
	}

	float Camera::FieldOfView() const
	{
		return fieldOfView;
	}

	void Camera::SetFieldOfView(float fieldOfView)
	{
		this->fieldOfView = fieldOfView;
		SetProjectionDirtyFlag();
	}

	void Camera::ResetAspectRatio()
	{
		aspectRatio = WindowApi::GetWindow()->GetAspectRatio();
		SetWasAspectRatioSetFlag();
		SetProjectionDirtyFlag();
	}

	float Camera::GetAspectRatio() const
	{
		return aspectRatio;
	}

	void Camera::SetAspectRatio(float width, float height)
	{
		this->aspectRatio = width / height;
		SetWasAspectRatioSetFlag();
		SetProjectionDirtyFlag();
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
		isPerspectiveProjection = enablePerspectiveProjection;
		SetProjectionDirtyFlag();
	}

	Camera* Camera::GetMainCamera()
	{
		return mainCamera;
	}

	void Camera::SetMainCamera(Camera& camera)
	{
		mainCamera = &camera;
	}

	std::shared_ptr<IRenderTexture> Camera::GetTargetTexture() const
	{
		return targetTexture;
	}

	void Camera::SetTargetTexture(const std::shared_ptr<IRenderTexture>& targetTexture)
	{
		this->targetTexture = targetTexture;
	}

	void Camera::Remove()
	{
	}

	void Camera::SetProjectionDirtyFlag()
	{
		isProjectionDirty = true;
	}

	void Camera::SetWasAspectRatioSetFlag()
	{
		wasAspectRatioSet = true;
	}

	START_REFLECTION(Camera)
	CLASS_MEMBER_REFLECTION(projectionMatrix)
	CLASS_MEMBER_REFLECTION(fieldOfView)
	CLASS_MEMBER_REFLECTION(clippingPlanes)
	CLASS_MEMBER_REFLECTION(isPerspectiveProjection)
	CLASS_MEMBER_REFLECTION(orthographicSize)
	CLASS_MEMBER_REFLECTION(isProjectionDirty)
	END_REFLECTION(Camera)
}
