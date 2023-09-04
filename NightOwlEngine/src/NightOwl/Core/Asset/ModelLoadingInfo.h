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

		std::vector<Vec3F> vertices;

		std::vector<Vec2F> uvs;

		std::vector<Vec3UI> indices;

		std::vector<Vec3F> normals;

		std::vector<Vec3F> tangents;

		std::vector<Vec3F> bitangents;

		std::vector<BoneWeight> boneWeights;

		int numberOfMeshesProcessed = 0;
	};
}
