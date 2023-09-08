#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"
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

		std::shared_ptr<Renderer> renderer;

		std::vector<BoneWeight> boneWeights;

		std::vector<GameObject> skeleton;

		int numberOfMeshesProcessed = 0;
	};
}
