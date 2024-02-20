#include "NightOwlPch.h"

#include "Light.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/System/LightSystem.h"
#include "NightOwl/Graphics/Types/GraphicsFormat.h"

namespace NightOwl
{
	Light::Light()
		: Component(ComponentType::Light),
		  range(1.0f),
		  intensity(1.0f),
		  shadowProjectionClippingPlanes(0.1f, 100.0f),
		  shadowOrthographicProjectionSize(15.0f),
		  shadowBias(0.05f),
		  shadowResolution(4096),
		  color(1.0f),
		  shadows(LightShadows::None),
		  type(LightType::Point),
		  shadowFrameBuffer(nullptr)
	{
		this->gameObject = gameObject;

		UpdateShadowProjectionMatrix();

		LightSystemLocator::Get()->AddLight(this);
	}

	float Light::GetRange() const
	{
		return range;
	}

	void Light::SetRange(float range)
	{
		this->range = range;
	}

	float Light::GetIntensity() const
	{
		return intensity;
	}

	void Light::SetIntensity(float intensity)
	{
		this->intensity = intensity;
	}

	Vec3F Light::GetColor() const
	{
		return color;
	}

	void Light::SetColor(const Vec3F& color)
	{
		this->color = color;
	}

	void Light::Remove()
	{
		LightSystemLocator::Get()->RemoveLight(this);

		if (this == globalLight)
		{
			globalLight = nullptr;
		}
	}

	LightShadows Light::GetShadows() const
	{
		return shadows;
	}

	void Light::SetShadows(LightShadows shadows)
	{
		this->shadows = shadows;

		if (shadows == LightShadows::None)
		{
			shadowFrameBuffer.reset();
			return;
		}

		if (shadowFrameBuffer == nullptr)
		{
			shadowFrameBuffer = Graphics::CreateRenderTexture(shadowResolution, shadowResolution, TextureFilterMode::Point, GraphicsFormat::Depth32F);
		}
	}

	float Light::GetShadowBias() const
	{
		return shadowBias;
	}

	void Light::SetShadowBias(float shadowBias)
	{
		this->shadowBias = shadowBias;
	}

	int Light::GetShadowResolution() const
	{
		return shadowResolution;
	}

	void Light::SetShadowResolution(int shadowResolution)
	{
		if (this->shadowResolution == shadowResolution)
		{
			return;
		}

		this->shadowResolution = shadowResolution;

		const std::shared_ptr<ITexture2D> depthBufferAttachment = shadowFrameBuffer->GetDepthBuffer();

		depthBufferAttachment->Resize(shadowResolution, shadowResolution, depthBufferAttachment->GetGraphicsFormat());
	}

	Light* Light::GetGlobalLight()
	{
		return globalLight;
	}

	void Light::SetGlobalLight(Light* light)
	{
		globalLight = light;
	}

	LightType Light::GetType() const
	{
		return type;
	}

	void Light::SetType(LightType type)
	{
		this->type = type;
	}

	Mat4F Light::GetShadowViewProjectionMatrix()
	{
		if (isProjectionDirty)
		{
			UpdateShadowProjectionMatrix();
		}

		return shadowProjectionMatrix * GetShadowViewMatrix();
	}

	Mat4F Light::GetShadowViewMatrix() const
	{
		return gameObject->GetTransform()->GetWorldMatrix().GetInverse();
	}

	void Light::SetShadowOrthographicProjectionSize(float orthographicSize)
	{
		shadowOrthographicProjectionSize = orthographicSize;

		isProjectionDirty = true;
	}

	void Light::SetShadowProjectionClippingPlanes(const ClippingPlanes& clippingPlanes)
	{
		shadowProjectionClippingPlanes = clippingPlanes;

		isProjectionDirty = true;
	}

	void Light::SetShadowProjectionNearPlane(float near)
	{
		shadowProjectionClippingPlanes.near = near;

		isProjectionDirty = true;
	}

	void Light::SetShadowProjectionFarPlane(float far)
	{
		shadowProjectionClippingPlanes.far = far;

		isProjectionDirty = true;
	}

	std::shared_ptr<IRenderTexture> Light::GetShadowFrameBuffer() const
	{
		return shadowFrameBuffer;
	}

	void Light::UpdateShadowProjectionMatrix()
	{
		isProjectionDirty = false;

		const float height = shadowOrthographicProjectionSize * 2.0f;
		// Since shadow aspect ratio is always 1, not need to multiply by shadow map aspect ratio
		const float width = height;

		const float halfHeight = height / 2.0f;
		const float halfWidth = width / 2.0f;

		shadowProjectionMatrix = Mat4F::Orthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, shadowProjectionClippingPlanes.near, shadowProjectionClippingPlanes.far);
	}
}
