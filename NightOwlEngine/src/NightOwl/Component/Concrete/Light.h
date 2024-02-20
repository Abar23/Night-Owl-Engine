#pragma once

#include "NightOwl/Component/Component.h"
#include "NightOwl/Component/Structures/ClippingPlanes.h"
#include "NightOwl/Component/Types/LightShadows.h"
#include "NightOwl/Component/Types/LightType.h"
#include "NightOwl/Math/Mat4.h"
#include "NightOwl/Math/Vec3.h"

namespace NightOwl
{
	class IRenderTexture;

	class Light : public Component
	{
	public:
		Light();

		float GetRange() const;

		void SetRange(float range);

		float GetIntensity() const;

		void SetIntensity(float intensity);

		Vec3F GetColor() const;

		void SetColor(const Vec3F& color);

		LightShadows GetShadows() const;

		void SetShadows(LightShadows shadows);

		float GetShadowBias() const;

		void SetShadowBias(float shadowBias);

		int GetShadowResolution() const;

		void SetShadowResolution(int shadowResolution);

		static Light* GetGlobalLight();

		static void SetGlobalLight(Light* light);

		LightType GetType() const;

		void SetType(LightType type);

		Mat4F GetShadowViewProjectionMatrix();

		Mat4F GetShadowViewMatrix() const;

		void SetShadowOrthographicProjectionSize(float orthographicSize);

		void SetShadowProjectionClippingPlanes(const ClippingPlanes& clippingPlanes);

		void SetShadowProjectionNearPlane(float near);

		void SetShadowProjectionFarPlane(float far);

		std::shared_ptr<IRenderTexture> GetShadowFrameBuffer() const;

	protected:
		void Remove() override;

	private:
		inline static Light* globalLight{ nullptr };

		float range;

		float intensity;

		ClippingPlanes shadowProjectionClippingPlanes;

		float shadowOrthographicProjectionSize;

		float shadowBias;

		int shadowResolution;

		bool isProjectionDirty;

		Vec3F color;

		LightShadows shadows;

		LightType type;

		Mat4F shadowProjectionMatrix;

		std::shared_ptr<IRenderTexture> shadowFrameBuffer;

		void UpdateShadowProjectionMatrix();
	};
}
