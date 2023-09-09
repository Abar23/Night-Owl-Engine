#pragma once

#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/GameObject/GameObject.h"
#include <vector>

namespace NightOwl
{
	class Model
	{
	public:
		std::shared_ptr<Renderer> renderer;

		std::vector<GameObject> skeleton;
	};
}

