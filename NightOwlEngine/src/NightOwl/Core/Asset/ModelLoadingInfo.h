#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
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

		std::shared_ptr<Mesh> modelMesh = nullptr;

		std::vector<BoneWeight> boneWeights;

		int numberOfMeshesProcessed = 0;
	};
}
