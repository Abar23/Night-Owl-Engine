#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"

namespace BrickBreak
{
	class BrickBreak
	{
	public:
		void Init();

		void Run();

	private:
		NightOwl::Core::Scene scene;
	};
}

