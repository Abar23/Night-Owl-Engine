#pragma once

#include <vector>

#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/GameObject/GameObject.h"

namespace NightOwl
{
	class Model
	{
	public:
		std::shared_ptr<Renderer> renderer;

		std::vector<GameObject> skeleton;
	};
}

