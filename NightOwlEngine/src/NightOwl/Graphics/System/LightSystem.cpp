#include "NightOwlPch.h"

#include "LightSystem.h"

#include "NightOwl/Component/Concrete/Light.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Graphics/Graphics.h"
#include "NightOwl/Graphics/Interfaces/IGraphicsBuffer.h"
#include "NightOwl/Graphics/Structures/LightGraphicsStruct.h"

namespace NightOwl
{
	LightSystem::LightSystem()
		: isDirty(false)
	{ }

	void LightSystem::Initialize()
	{
		lightGraphicsBuffer = Graphics::CreateGraphicsBuffer(BufferType::Storage);
	}

	void LightSystem::SetupLightBuffer()
	{
		if (isDirty == false)
		{
			return;
		}
		isDirty = false;

		lightData.resize(lights.size());

		// TODO: make = operator for LightGraphicsStruct to Light
		for (int lightIndex = 0; lightIndex < lights.size(); ++lightIndex)
		{
			lightData[lightIndex].position = lights[lightIndex]->GetGameObject().GetTransform()->GetPosition();
			lightData[lightIndex].color = lights[lightIndex]->GetColor();
		}

		lightGraphicsBuffer->SetSize(lightData.size(), sizeof(LightGraphicsStruct));
		lightGraphicsBuffer->SetData(lightData.data());
	}

	std::shared_ptr<IGraphicsBuffer> LightSystem::GetLightBuffer()
	{
		return lightGraphicsBuffer;
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
		lightGraphicsBuffer.reset();
	}
}
