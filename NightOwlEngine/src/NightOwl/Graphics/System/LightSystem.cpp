#include "NightOwlPch.h"

#include "LightSystem.h"

#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Interfaces/IGraphicsBuffer.h"
#include "NightOwl/Graphics/Structures/GraphicsLight.h"

namespace NightOwl
{
	LightSystem::LightSystem()
		: isDirty(false)
	{ }

	void LightSystem::Initialize()
	{
		pointLightGraphicsBuffer = Graphics::CreateGraphicsBuffer(BufferType::Storage);
		directionalLightGraphicsBuffer = Graphics::CreateGraphicsBuffer(BufferType::Storage);
		globalDirectionalLightGraphicsBuffer = Graphics::CreateGraphicsBuffer(BufferType::Storage);
	}

	void LightSystem::SetupLightBuffers()
	{
		if (isDirty == false)
		{
			return;
		}
		isDirty = false;

		pointLightData.clear();
		directionalLightData.clear();

		// Setup point light data for shader storage block
		const Light* globalLight = Light::GetGlobalLight();
		if (globalLight != nullptr && globalLight->GetType() == LightType::Directional)
		{
			const GraphicsDirectionalLight globalDirectionalLight = globalLight;
			globalDirectionalLightGraphicsBuffer->SetSize(1, sizeof(GraphicsDirectionalLight));
			globalDirectionalLightGraphicsBuffer->SetData(&globalDirectionalLight);
		}

		for (const auto* light : lights)
		{
			if (light == globalLight)
			{
				continue;
			}

			if (light->GetType() == LightType::Point)
			{
				GraphicsPointLight graphicsPointLight = light;
				pointLightData.push_back(graphicsPointLight);
				continue;
			}

			if (light->GetType() == LightType::Directional)
			{
				GraphicsDirectionalLight graphicsDirectionalLight = light;
				directionalLightData.push_back(graphicsDirectionalLight);
			}
		}

		pointLightGraphicsBuffer->SetSize(pointLightData.size(), sizeof(GraphicsPointLight));
		pointLightGraphicsBuffer->SetData(pointLightData.data());

		directionalLightGraphicsBuffer->SetSize(directionalLightData.size(), sizeof(GraphicsDirectionalLight));
		directionalLightGraphicsBuffer->SetData(directionalLightData.data());
	}

	std::shared_ptr<IGraphicsBuffer> LightSystem::GetPointLightBuffer()
	{
		return pointLightGraphicsBuffer;
	}

	std::shared_ptr<IGraphicsBuffer> LightSystem::GetDirectionalLightBuffer()
	{
		return directionalLightGraphicsBuffer;
	}

	std::shared_ptr<IGraphicsBuffer> LightSystem::GetGlobalLightBuffer()
	{
		return globalDirectionalLightGraphicsBuffer;
	}

	void LightSystem::AddLight(Light* light)
	{
		isDirty = true;

		lights.push_back(light);
	}

	const std::vector<Light*>& LightSystem::GetLights()
	{
		return lights;
	}

	void LightSystem::RemoveLight(const Light* light)
	{
		isDirty = true;

		int lightIndex = 0;
		for (const auto lightComponentPointer : lights)
		{
			if (lightComponentPointer == light)
			{
				break;
			}
			lightIndex++;
		}

		if (lightIndex == lights.size())
		{
			return;
		}

		std::iter_swap(lights.begin() + lightIndex, lights.end() - 1);
		lights.erase(lights.end() - 1);
	}

	void LightSystem::ShutDown()
	{
		pointLightGraphicsBuffer.reset();
		directionalLightGraphicsBuffer.reset();
		globalDirectionalLightGraphicsBuffer.reset();
	}
}
