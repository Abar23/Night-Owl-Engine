#pragma once

#include "NightOwl/Component/Structures/Mesh.h"
#include "ModelLoadingInfo.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include <string>

namespace NightOwl
{
	class Mesh;

	class AssimpModelLoader
	{
	public:
		static void LoadModel(const std::string& filePath);

		static void LoadAnimation(const std::string& filePath);

	private:
		static Assimp::Importer assimpImporter;

		static void ProcessNodes(ModelLoadingInfo& modelLoadingInfo);

		static void ProcessMesh(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh);

		static void ProcessBones(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh);

		static void ProcessMaterials(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh);

		static void ProcessArmature(ModelLoadingInfo& modelLoadingInfo);

		static void ProcessAnimation(const aiAnimation* assimpAnimation);

		static bool HasBones(const ModelLoadingInfo& modelLoadingInfo);
	};
}
