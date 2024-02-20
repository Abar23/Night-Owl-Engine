#include "NightOwlPch.h"

#include "GraphicsLight.h"
#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	GraphicsPointLight::GraphicsPointLight(const Light* light)
		: position(light->GetGameObject().GetTransform()->GetPosition()),
		  color(light->GetColor()),
		  range(light->GetRange()),
		  intensity(light->GetIntensity()),
		  shadowBias(light->GetShadowBias())
	{
	}

	GraphicsPointLight& GraphicsPointLight::operator=(const Light* light)
	{
		position = light->GetGameObject().GetTransform()->GetPosition();
		color = light->GetColor();
		range = light->GetRange();
		intensity = light->GetIntensity();
		shadowBias = light->GetShadowBias();

		return *this;
	}

	GraphicsDirectionalLight::GraphicsDirectionalLight(const Light* light)
		: direction(light->GetGameObject().GetTransform()->GetForward()),
		  color(light->GetColor()),
		  intensity(light->GetIntensity()),
		  shadowBias(light->GetShadowBias())
	{ }

	GraphicsDirectionalLight& GraphicsDirectionalLight::operator=(const Light* light)
	{
		direction = light->GetGameObject().GetTransform()->GetForward();
		color = light->GetColor();
		intensity = light->GetIntensity();
		shadowBias = light->GetShadowBias();

		return *this;
	}
}
