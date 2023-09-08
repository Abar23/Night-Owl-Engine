#include "NightOwlPch.h"

#include "AssimpModelLoader.h"
#include "assimp/postprocess.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include <stack>

#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/GameObject/GameObject.h"


namespace NightOwl
{
	Assimp::Importer AssimpModelLoader::modelImporter;

	void AssimpModelLoader::LoadModel(const std::string& filePath)
	{
		ModelLoadingInfo modelLoadingInfo;

		modelLoadingInfo.filePath = filePath;
		modelLoadingInfo.scene = modelImporter.ReadFile(modelLoadingInfo.filePath, aiProcess_Triangulate | aiProcess_PopulateArmatureData | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
		modelLoadingInfo.name = Utility::StripFilePathToNameWithoutExtension(modelLoadingInfo.filePath);
		modelLoadingInfo.directory = Utility::FilePathToDirectory(modelLoadingInfo.filePath);

		if (modelLoadingInfo.scene && 
			modelLoadingInfo.scene->mRootNode && 
			modelLoadingInfo.scene->mRootNode->mChildren)
		{
			ProcessNodes(modelLoadingInfo);
		}
	}

	void AssimpModelLoader::ProcessNodes(ModelLoadingInfo& modelLoadingInfo)
	{
		const std::shared_ptr<Model> model = std::make_shared<Model>();

		const aiScene* scene = modelLoadingInfo.scene;

		if(HasBones(modelLoadingInfo, scene->mRootNode->mChildren[0]))
		{
			modelLoadingInfo.renderer = std::make_shared<SkinnedMeshRenderer>();
			ProcessArmature(modelLoadingInfo);
		}
		else
		{
			modelLoadingInfo.renderer = std::make_shared<MeshRenderer>();
		}

		modelLoadingInfo.renderer->mesh = std::make_shared<Mesh>();
		modelLoadingInfo.renderer->Remove();

		model->skeleton.swap(modelLoadingInfo.skeleton);
		model->renderer = modelLoadingInfo.renderer;

		for (int rootChildIndex = 0; rootChildIndex < scene->mRootNode->mNumChildren; ++rootChildIndex)
		{
			const aiNode* child = scene->mRootNode->mChildren[rootChildIndex];
			// Mesh with the armature/skeleton
			if (child->mNumMeshes == 0)
			{
				continue;
			}
			
			std::stack<const aiNode*> nodes;
			nodes.push(child);

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
		}

		modelLoadingInfo.renderer->mesh->ValidateMesh();
		modelLoadingInfo.renderer->mesh->UploadMeshData();
		AssetManagerLocator::GetAssetManager()->GetModelRepository().AddAsset(modelLoadingInfo.name, model);
	}

	void AssimpModelLoader::ProcessMesh(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		std::shared_ptr<Mesh> modelMesh = modelLoadingInfo.renderer->mesh;
		if (modelLoadingInfo.numberOfMeshesProcessed > 0)
		{
			SubMeshData subMesh;
			subMesh.baseVertex = modelMesh->vertices.size();
			subMesh.indexStart = modelMesh->triangles.size() * 3;
			subMesh.indexCount = assimpMesh->mNumFaces * 3;
			subMesh.vertexCount = assimpMesh->mNumVertices;
			subMesh.firstVertex = modelMesh->triangles[modelMesh->triangles.size() - 1].x;
			modelMesh->subMeshes.push_back(subMesh);
		}
		
		// process data for main mesh
		for (unsigned int vertexIndex = 0; vertexIndex < assimpMesh->mNumVertices; ++vertexIndex)
		{
			const aiVector3D assimpVertex = assimpMesh->mVertices[vertexIndex];
			modelMesh->vertices.emplace_back(assimpVertex.x, assimpVertex.y, assimpVertex.z);

			Vec2F textureCoordinates;
			// process texture coordinates first channel, some models have 8 channels
			if (assimpMesh->mTextureCoords[0])
			{
				const aiVector3D assimpTextureCoordinate = assimpMesh->mTextureCoords[0][vertexIndex];
				textureCoordinates.x = assimpTextureCoordinate.x;
				textureCoordinates.y = assimpTextureCoordinate.y;
			}
			modelMesh->uvs.push_back(textureCoordinates);

			Vec3F normal;
			if (assimpMesh->HasNormals())
			{
				const aiVector3D assimpNormal = assimpMesh->mNormals[vertexIndex];
				normal.x = assimpNormal.x;
				normal.y = assimpNormal.y;
				normal.z = assimpNormal.z;
			}
			modelMesh->normals.push_back(normal);

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
			modelMesh->bitangents.push_back(bitangent);
			modelMesh->tangents.push_back(tangent);
		}

		for (unsigned int faceIndicesIndex = 0; faceIndicesIndex < assimpMesh->mNumFaces; ++faceIndicesIndex)
		{
			const aiFace face = assimpMesh->mFaces[faceIndicesIndex];
			if (face.mNumIndices != 3)
			{
				// engine assert since we only should process triangles
			}
			modelMesh->triangles.emplace_back(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}

		if (assimpMesh->HasBones())
		{
			ProcessBones(modelLoadingInfo, assimpMesh);
		}

		// const aiMaterial* material = modelLoadingInfo.scene->mMaterials[assimpMesh->mMaterialIndex];
		//
		// std::vector<ITexture2D*> diffuseTextures;
		// for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(aiTextureType_DIFFUSE); ++textureIndex)
		// {
		// 	aiString str;
		// 	material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &str);
		//
		// 	std::string textureFilePath(str.C_Str());
		// 	Utility::StandardizeFilePathString(textureFilePath);
		// 	textureFilePath = modelLoadingInfo.directory + '/' + textureFilePath;
		//
		// 	diffuseTextures.push_back(AssetManagerLocator::GetAssetManager()->LoadTexture2D(textureFilePath));
		// }

		modelLoadingInfo.numberOfMeshesProcessed++;
	}

	void AssimpModelLoader::ProcessBones(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		std::shared_ptr<Mesh>& modelMesh = modelLoadingInfo.renderer->mesh;

		modelLoadingInfo.boneWeights.resize(modelLoadingInfo.renderer->mesh->vertices.size());

		for (int boneIndex = 0; boneIndex < assimpMesh->mNumBones; ++boneIndex)
		{
			int boneId;
			const aiBone* bone = assimpMesh->mBones[boneIndex];
			const std::string boneName = bone->mName.C_Str();
			if (modelMesh->boneInfoMap.contains(boneName) == false)
			{
				BoneInfo boneInfo;
				boneId = modelMesh->boneInfoMap.size();
				boneInfo.id = boneId;
				boneInfo.offsetMatrix = Utility::AssimpMat4ToNightOwlMat4F(bone->mOffsetMatrix);
				modelMesh->boneInfoMap[boneName] = boneInfo;
			}
			else
			{
				boneId = modelMesh->boneInfoMap[boneName].id;
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

	void AssimpModelLoader::ProcessArmature(ModelLoadingInfo& modelLoadingInfo)
	{
		const aiScene* scene = modelLoadingInfo.scene;
		if (scene == nullptr ||
			scene->mNumMeshes == 0 ||
			scene->mMeshes[0]->mBones == nullptr ||
			scene->mMeshes[0]->mBones[0]->mArmature == nullptr)
		{
			return;
		}

		// adding one for the root node/bone
		const int numberOfBones = scene->mMeshes[0]->mNumBones + 1;
		const aiNode* armatureRoot = scene->mMeshes[0]->mBones[0]->mArmature;

		modelLoadingInfo.skeleton.resize(numberOfBones);

		std::stack<std::pair<GameObject*, const aiNode*>> armatureNodes;
		armatureNodes.emplace(modelLoadingInfo.skeleton.data(), armatureRoot);

		int processedGameObjects = 0;
		while (armatureNodes.empty() == false)
		{
			const std::pair<GameObject*, const aiNode*> armatureNodePair = armatureNodes.top();
			armatureNodes.pop();

			armatureNodePair.first->SetName(armatureNodePair.second->mName.data);
			ENGINE_LOG_INFO("Game Object Cloned: {0}", armatureNodePair.first->GetName());

			if (armatureNodePair.first->GetName() == "ik_foot_root")
			{
				int i = 0;
			}

			aiVector3D scale;
			aiVector3D position;
			aiQuaternion rotation;
			armatureNodePair.second->mTransformation.Decompose(scale, rotation, position);

			Transform* parentGameObjectTransform = armatureNodePair.first->GetTransform();
			parentGameObjectTransform->SetLocalScale(Utility::AssimpVec3ToNightOwlVec3F(scale));
			parentGameObjectTransform->SetLocalPosition(Utility::AssimpVec3ToNightOwlVec3F(position));
			parentGameObjectTransform->SetLocalRotation(Utility::AssimpQuaternionToNightOwlQuatF(rotation));

			for (unsigned int armatureChildIndex = 0; armatureChildIndex < armatureNodePair.second->mNumChildren; ++armatureChildIndex)
			{
				processedGameObjects++;

				if (processedGameObjects == 67)
				{
					int i = 0;
				}

				GameObject* childGameObject = &modelLoadingInfo.skeleton.at(processedGameObjects);
				childGameObject->GetTransform()->SetParent(parentGameObjectTransform);

				armatureNodes.emplace(childGameObject, armatureNodePair.second->mChildren[armatureChildIndex]);
			}
		}
	}

	bool AssimpModelLoader::HasBones(ModelLoadingInfo& modelLoadingInfo, const aiNode* assimpNode)
	{
		const aiScene* scene = modelLoadingInfo.scene;
		if (scene == nullptr)
		{
			return false;
		}

		if (assimpNode->mNumMeshes == 0)
		{
			return false;
		}

		const aiMesh* baseMesh = scene->mMeshes[assimpNode->mMeshes[0]];

		return baseMesh->HasBones();
	}
}
