#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include <string>
#include <assimp/scene.h>

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
