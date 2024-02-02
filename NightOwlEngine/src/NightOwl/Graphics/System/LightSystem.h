#pragma once

#include "NightOwl/Graphics/Structures/LightGraphicsStruct.h"
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

		void SetupLightBuffer();

		std::shared_ptr<IGraphicsBuffer> GetLightBuffer();

		void AddLight(Light* light);

		void RemoveLight(const Light* light);

	private:
		std::vector<Light*> lights;

		std::vector<LightGraphicsStruct> lightData;

		std::shared_ptr<IGraphicsBuffer> lightGraphicsBuffer;

		bool isDirty;
	};
}
