#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include <string>

#include "ModelLoadingInfo.h"

namespace NightOwl
{
	class Mesh;

	class AssimpModelLoader
	{
	public:
		static void LoadModel(const std::string& filePath);

		static void ProcessNodes(ModelLoadingInfo& modelLoadingInfo);

		static void ProcessMesh(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh);

	private:
		static Assimp::Importer modelImporter;
	};
}
