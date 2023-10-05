#pragma once

#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/GameObject/GameObject.h"
#include <string>

struct aiScene;

namespace NightOwl
{
	class ModelLoadingInfo
	{
	public:
		std::string filePath;

		std::string directory;

		std::string name;

		const aiScene* scene;

		std::shared_ptr<Model> model;
	};
}
