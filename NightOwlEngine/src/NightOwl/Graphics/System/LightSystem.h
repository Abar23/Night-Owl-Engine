#pragma once

#include "NightOwl/Graphics/Structures/GraphicsLight.h"
#include <memory>
#include <vector>

namespace NightOwl
{
	class IGraphicsBuffer;

	class Light;

	class LightSystem
	{
	public:
		LightSystem();

		void Initialize();

		void SetupLightBuffers();

		std::shared_ptr<IGraphicsBuffer> GetPointLightBuffer();

		std::shared_ptr<IGraphicsBuffer> GetDirectionalLightBuffer();

		std::shared_ptr<IGraphicsBuffer> GetGlobalLightBuffer();

		void AddLight(Light* light);

		const std::vector<Light*>& GetLights();

		void RemoveLight(const Light* light);

		void ShutDown();

	private:
		std::vector<Light*> lights;

		std::vector<GraphicsPointLight> pointLightData;

		std::vector<GraphicsDirectionalLight> directionalLightData;

		std::shared_ptr<IGraphicsBuffer> pointLightGraphicsBuffer;

		std::shared_ptr<IGraphicsBuffer> directionalLightGraphicsBuffer;

		std::shared_ptr<IGraphicsBuffer> globalDirectionalLightGraphicsBuffer;

		bool isDirty;
	};
}
