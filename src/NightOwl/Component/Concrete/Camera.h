#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Structures/CameraStructs.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl::Component
{
	class Camera : public Component
	{
	public:
		Camera(GameObject::GameObject& gameObject);

		void LookAt(Math::Vec3F pointToLookAt);

		Math::Mat4F ViewProjectionMatrix();

		const Math::Mat4F& GetViewMatrix();

		const Math::Mat4F& GetViewMatrix() const;

		float GetNearClippingPlane();

		void SetNearClippingPlane(float nearPlaneDistance);

		float GetFarClippingPlane();

		void SetFarClippingPlane(float farPlaneDistance);

		ClippingPlanes GetClippingPlanes();

		void SetClippingPlanes(const ClippingPlanes& clippingPlanes);

		float FieldOfView() const;

		void SetFieldOfView(float fieldOfView);

		//void SetAspectRatioToWindows();

		//float GetAspectRatio() const;

		//void SetAspectRatio(float aspectRatio);

		float GetOrthographicSize() const;

		void SetOrthographicSize(float orthographicSize);

		bool IsPerspectiveProjection();

		void SetPerspectiveMode(bool enablePerspectiveProjection);

		static Camera* GetMainCamera();

	private:
		static Camera* mainCamera;

		Math::Mat4F projectionMatrix;

		float fieldOfView;

		ClippingPlanes clippingPlanes;

		bool isPerspectiveProjection;

		//float aspectRatio;

		float orthographicSize;

		//bool isAspectRatioDirty;

		bool isProjectionDirty;

		//void SetAspectRatioDirtyFlag();

		void SetProjectionDirtyFlag();
	};
}

