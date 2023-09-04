#include "NightOwlPch.h"

#include "AssimpModelLoader.h"
#include "assimp/postprocess.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include <stack>

namespace NightOwl
{
	Assimp::Importer AssimpModelLoader::modelImporter;

	void AssimpModelLoader::LoadModel(const std::string& filePath)
	{
		ModelLoadingInfo modelLoadingInfo;

		modelLoadingInfo.filePath = filePath;
		modelLoadingInfo.scene = modelImporter.ReadFile(modelLoadingInfo.filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
		modelLoadingInfo.name = Utility::StripFilePathToNameWithoutExtension(modelLoadingInfo.filePath);
		modelLoadingInfo.directory = Utility::FilePathToDirectory(modelLoadingInfo.filePath);

		ProcessNodes(modelLoadingInfo);
	}

	void AssimpModelLoader::ProcessNodes(ModelLoadingInfo& modelLoadingInfo)
	{
		const aiScene* scene = modelLoadingInfo.scene;
		if (scene->HasMeshes() == false)
		{
			return;
		}

		modelLoadingInfo.modelMesh = std::make_shared<Mesh>();

		std::stack<const aiNode*> nodes;
		nodes.push(scene->mRootNode);

		// iterate through all nodes of the model
		while (nodes.empty() != true)
		{
			const aiNode* currentNode = nodes.top();
			nodes.pop();
			
			// process all meshes in the model
			for (unsigned int meshIndex = 0; meshIndex < currentNode->mNumMeshes; meshIndex++)
			{
				const aiMesh* assimpMesh = scene->mMeshes[currentNode->mMeshes[meshIndex]];
				ProcessMesh(modelLoadingInfo, assimpMesh);
			}

			// get next children
			for (unsigned int childIndex = 0; childIndex < currentNode->mNumChildren; ++childIndex)
			{
				nodes.push(currentNode->mChildren[childIndex]);
			}
		}

		modelLoadingInfo.modelMesh->vertices = modelLoadingInfo.vertices;
		modelLoadingInfo.modelMesh->uvs = modelLoadingInfo.uvs;
		modelLoadingInfo.modelMesh->triangles = modelLoadingInfo.indices;
		modelLoadingInfo.modelMesh->normals = modelLoadingInfo.normals;
		modelLoadingInfo.modelMesh->tangents = modelLoadingInfo.tangents;
		modelLoadingInfo.modelMesh->bitangents = modelLoadingInfo.bitangents;
		modelLoadingInfo.modelMesh->ValidateMesh();
		modelLoadingInfo.modelMesh->UploadMeshData();

		AssetManagerLocator::GetAssetManager()->GetModelRepository().AddAsset(modelLoadingInfo.name, modelLoadingInfo.modelMesh);
	}

	void AssimpModelLoader::ProcessMesh(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		if (modelLoadingInfo.numberOfMeshesProcessed > 0)
		{
			SubMeshData subMesh;
			subMesh.baseVertex = modelLoadingInfo.vertices.size();
			subMesh.indexStart = modelLoadingInfo.indices.size() * 3;
			subMesh.indexCount = assimpMesh->mNumFaces * 3;
			subMesh.vertexCount = assimpMesh->mNumVertices;
			subMesh.firstVertex = modelLoadingInfo.indices[modelLoadingInfo.indices.size() - 1].x;
			modelLoadingInfo.modelMesh->subMeshes.push_back(subMesh);
		}

		// process data for main mesh
		for (unsigned int vertexIndex = 0; vertexIndex < assimpMesh->mNumVertices; ++vertexIndex)
		{
			const aiVector3D assimpVertex = assimpMesh->mVertices[vertexIndex];
			modelLoadingInfo.vertices.emplace_back(assimpVertex.x, assimpVertex.y, assimpVertex.z);

			Vec2F textureCoordinates;
			// process texture coordinates first channel, some models have 8 channels
			if (assimpMesh->mTextureCoords[0])
			{
				const aiVector3D assimpTextureCoordinate = assimpMesh->mTextureCoords[0][vertexIndex];
				textureCoordinates.x = assimpTextureCoordinate.x;
				textureCoordinates.y = assimpTextureCoordinate.y;
			}
			modelLoadingInfo.uvs.push_back(textureCoordinates);

			Vec3F normal;
			if (assimpMesh->HasNormals())
			{
				const aiVector3D assimpNormal = assimpMesh->mNormals[vertexIndex];
				normal.x = assimpNormal.x;
				normal.y = assimpNormal.y;
				normal.z = assimpNormal.z;
			}
			modelLoadingInfo.normals.push_back(normal);

			Vec3F bitangent;
			Vec3F tangent;
			if (assimpMesh->HasTangentsAndBitangents())
			{
				aiVector3D assimpNormal = assimpMesh->mBitangents[vertexIndex];
				bitangent.x = assimpNormal.x;
				bitangent.y = assimpNormal.y;
				bitangent.z = assimpNormal.z;

				assimpNormal = assimpMesh->mTangents[vertexIndex];
				tangent.x = assimpNormal.x;
				tangent.y = assimpNormal.y;
				tangent.z = assimpNormal.z;
			}
			modelLoadingInfo.bitangents.push_back(bitangent);
			modelLoadingInfo.tangents.push_back(tangent);
		}

		std::vector<Vec3UI> faceIndices;
		for (unsigned int faceIndicesIndex = 0; faceIndicesIndex < assimpMesh->mNumFaces; ++faceIndicesIndex)
		{
			const aiFace face = assimpMesh->mFaces[faceIndicesIndex];
			if (face.mNumIndices != 3)
			{
				// engine assert since we only should process triangles
			}
			modelLoadingInfo.indices.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}

		if (assimpMesh->HasBones())
		{
			ProcessBones(modelLoadingInfo, assimpMesh);
		}

		const aiMaterial* material = modelLoadingInfo.scene->mMaterials[assimpMesh->mMaterialIndex];
		
		std::vector<ITexture2D*> diffuseTextures;
		for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(aiTextureType_DIFFUSE); ++textureIndex)
		{
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &str);

			std::string textureFilePath(str.C_Str());
			Utility::StandardizeFilePathString(textureFilePath);
			textureFilePath = modelLoadingInfo.directory + '/' + textureFilePath;

			diffuseTextures.push_back(AssetManagerLocator::GetAssetManager()->LoadTexture2D(textureFilePath));
		}

		modelLoadingInfo.numberOfMeshesProcessed++;
	}

	void AssimpModelLoader::ProcessBones(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		modelLoadingInfo.boneWeights.resize(modelLoadingInfo.vertices.size());

		for (int boneIndex = 0; boneIndex < assimpMesh->mNumBones; ++boneIndex)
		{
			int boneId;
			const aiBone* bone = assimpMesh->mBones[boneIndex];
			const std::string boneName = bone->mName.C_Str();
			if (modelLoadingInfo.modelMesh->boneInfoMap.contains(boneName) == false)
			{
				BoneInfo boneInfo;
				boneId = modelLoadingInfo.modelMesh->boneInfoMap.size();
				boneInfo.id = boneId;
				boneInfo.offsetMatrix = Utility::AssimpMat4ToNightOwlMat4(bone->mOffsetMatrix);
				modelLoadingInfo.modelMesh->boneInfoMap[boneName] = boneInfo;
			}
			else
			{
				boneId = modelLoadingInfo.modelMesh->boneInfoMap[boneName].id;
			}

			// assert for valid bone index

			for (int weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex)
			{
				const aiVertexWeight& weight = bone->mWeights[weightIndex];
				BoneWeight* boneWeight = modelLoadingInfo.boneWeights.data() + weight.mVertexId;

				for (int boneWeightIndex = 0; boneWeightIndex < 4; ++boneWeightIndex)
				{
					if (boneWeight->boneIds[boneWeightIndex] < 0)
					{
						boneWeight->boneIds[boneWeightIndex] = boneId;
						boneWeight->weights[boneWeightIndex] = weight.mWeight;
						break;
					}
				}
			}
		}
	}
}
