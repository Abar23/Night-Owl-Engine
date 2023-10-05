#pragma once

#include "NightOwl/GameObject/GameObject.h"
#include <vector>

namespace NightOwl
{
	class Renderer;

	class Model
	{
	public:
		std::shared_ptr<Renderer> renderer;

		std::vector<GameObject> skeleton;
	};
}

