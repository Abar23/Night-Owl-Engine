#pragma once

#include "ModelLoadingInfo.h"
#include "NightOwl/Component/Structures/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <string>

namespace NightOwl
{
	class Material;

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

		static void ProcessMaterials(ModelLoadingInfo& modelLoadingInfo, const aiMaterial* assimpMaterial, const unsigned int materialIndex);

		static void LoadTextureFromMaterial(const ModelLoadingInfo& modelLoadingInfo, const std::shared_ptr<Material>& rendererMaterial, const aiMaterial* assimpMaterial, aiTextureType textureType, const std::string& uniformName);

		static void ProcessArmature(ModelLoadingInfo& modelLoadingInfo);

		static void ProcessAnimation(const aiAnimation* assimpAnimation, const std::string& animationName);

		static bool HasBones(const ModelLoadingInfo& modelLoadingInfo);
	};
}
