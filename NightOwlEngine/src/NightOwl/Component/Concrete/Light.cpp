#include "NightOwlPch.h"

#include "Light.h"
#include "NightOwl/Core/Locator/Locator.h"
#include "NightOwl/Graphics/System/LightSystem.h"

namespace NightOwl
{
	Light::Light()
		: Component(ComponentType::Light),
		  range(1.0f),
		  intensity(1.0f)
	{
		this->gameObject = gameObject;

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
	}
}
