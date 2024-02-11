#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Structures/ClippingPlanes.h"
#include "NightOwl/Core/Reflection/Reflection.h"
#include "NightOwl/Math/Math.h"

namespace NightOwl
{
	class IRenderTexture;

	class Camera : public Component
	{
	public:
		Camera();

		std::shared_ptr<Component> Clone() override;

		Mat4F GetViewProjectionMatrix();

		Mat4F GetViewMatrix() const;

		float GetNearClippingPlane() const;

		void SetNearClippingPlane(float nearPlaneDistance);

		float GetFarClippingPlane() const;

		void SetFarClippingPlane(float farPlaneDistance);

		ClippingPlanes GetClippingPlanes();

		void SetClippingPlanes(const ClippingPlanes& clippingPlanes);

		float FieldOfView() const;

		void SetFieldOfView(float fieldOfView);

		void ResetAspectRatio();

		float GetAspectRatio() const;

		void SetAspectRatio(float width, float height);

		float GetOrthographicSize() const;

		void SetOrthographicSize(float orthographicSize);

		bool IsPerspectiveProjection();

		void SetPerspectiveMode(bool enablePerspectiveProjection);

		static Camera* GetMainCamera();

		static void SetMainCamera(Camera& camera);

		IRenderTexture* GetTargetTexture() const;

		void SetTargetTexture(IRenderTexture* targetTexture);

		void Remove() override;

		REFLECT()

	private:
		static Camera* mainCamera;

		Mat4F projectionMatrix;

		float fieldOfView;

		ClippingPlanes clippingPlanes;

		float aspectRatio;

		float orthographicSize;

		bool isPerspectiveProjection;

		bool wasAspectRatioSet;

		bool isProjectionDirty;

		IRenderTexture* targetTexture;

		void SetProjectionDirtyFlag();

		void SetWasAspectRatioSetFlag();
	};
}
