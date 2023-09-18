#include "NightOwlPch.h"

#include "AssimpModelLoader.h"
#include "assimp/postprocess.h"
#include "NightOwl/Component/Abstrtact/Renderer.h"
#include "NightOwl/Core/Locator/AssetManagerLocator.h"
#include "NightOwl/Core/Utitlity/Utils.h"
#include "NightOwl/Animation/3D/Structures/Model.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/SkinnedMeshRenderer.h"
#include "NightOwl/GameObject/GameObject.h"
#include <stack>

#include "NightOwl/Graphics/Materials/Material.h"

namespace NightOwl
{
	Assimp::Importer AssimpModelLoader::assimpImporter;

	void AssimpModelLoader::LoadModel(const std::string& filePath)
	{
		ModelLoadingInfo modelLoadingInfo;

		modelLoadingInfo.filePath = filePath;
		assimpImporter.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		modelLoadingInfo.scene = assimpImporter.ReadFile(modelLoadingInfo.filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_PopulateArmatureData | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
		modelLoadingInfo.name = Utility::StripFilePathToNameWithoutExtension(modelLoadingInfo.filePath);
		modelLoadingInfo.directory = Utility::FilePathToDirectory(modelLoadingInfo.filePath);

		if (modelLoadingInfo.scene && 
			modelLoadingInfo.scene->mRootNode && 
			modelLoadingInfo.scene->mRootNode->mChildren)
		{
			ProcessNodes(modelLoadingInfo);
		}

		assimpImporter.FreeScene();
	}

	void AssimpModelLoader::LoadAnimation(const std::string& filePath)
	{
		const aiScene* scene = assimpImporter.ReadFile(filePath, aiProcess_Triangulate);

		if (scene->HasAnimations() == false)
		{
			return;
		}

		for (unsigned int animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex)
		{
			ProcessAnimation(scene->mAnimations[animationIndex]);
		}
	}

	void AssimpModelLoader::ProcessNodes(ModelLoadingInfo& modelLoadingInfo)
	{
		AssetManager* assetManager = AssetManagerLocator::GetAssetManager();

		modelLoadingInfo.model = std::make_shared<Model>();

		std::shared_ptr<Model> model = modelLoadingInfo.model;

		const aiScene* scene = modelLoadingInfo.scene;

		if(HasBones(modelLoadingInfo))
		{
			model->renderer = std::make_shared<SkinnedMeshRenderer>();
			ProcessArmature(modelLoadingInfo);
		}
		else
		{
			model->renderer = std::make_shared<MeshRenderer>();
		}
		model->renderer->mesh = std::make_shared<Mesh>();
		model->renderer->Remove();

		if (scene->HasAnimations())
		{
			for (unsigned int animationIndex = 0; animationIndex < scene->mNumAnimations; ++animationIndex)
			{
				const aiAnimation* animation = scene->mAnimations[animationIndex];
				if (assetManager->GetAnimationRepository().HasAsset(animation->mName.data))
				{
					continue;
				}

				ProcessAnimation(scene->mAnimations[animationIndex]);
			}
		}

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

		model->renderer->mesh->ValidateMesh();
		model->renderer->mesh->UploadMeshData();
		assetManager->GetModelRepository().AddAsset(modelLoadingInfo.name, model);
	}

	void AssimpModelLoader::ProcessMesh(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		std::shared_ptr<Mesh> modelMesh = modelLoadingInfo.model->renderer->mesh;

		SubMeshData subMesh;
		subMesh.baseVertex = modelMesh->vertices.size();
		subMesh.indexStart = modelMesh->triangles.size() * 3;
		subMesh.indexCount = assimpMesh->mNumFaces * 3;
		subMesh.vertexCount = assimpMesh->mNumVertices;
		modelMesh->subMeshes.push_back(subMesh);
		
		// process data for main mesh
		for (unsigned int vertexIndex = 0; vertexIndex < assimpMesh->mNumVertices; ++vertexIndex)
		{
			const aiVector3D assimpVertex = assimpMesh->mVertices[vertexIndex];
			modelMesh->vertices.emplace_back(assimpVertex.x, assimpVertex.y, assimpVertex.z);

			Vec2F textureCoordinates;
			// process texture coordinates first channel, some models have 8 channels
			if (assimpMesh->mTextureCoords[0])
			{
				const aiVector3D& assimpTextureCoordinate = assimpMesh->mTextureCoords[0][vertexIndex];
				textureCoordinates.x = assimpTextureCoordinate.x;
				textureCoordinates.y = assimpTextureCoordinate.y;
			}
			modelMesh->uvs.push_back(textureCoordinates);

			Vec3F normal;
			if (assimpMesh->HasNormals())
			{
				const aiVector3D& assimpNormal = assimpMesh->mNormals[vertexIndex];
				normal.x = assimpNormal.x;
				normal.y = assimpNormal.y;
				normal.z = assimpNormal.z;
			}
			modelMesh->normals.push_back(normal);

			Vec3F bitangent;
			Vec3F tangent;
			if (assimpMesh->HasTangentsAndBitangents())
			{
				aiVector3D& assimpNormal = assimpMesh->mBitangents[vertexIndex];
				bitangent.x = assimpNormal.x;
				bitangent.y = assimpNormal.y;
				bitangent.z = assimpNormal.z;

				aiVector3D& assimpTangent = assimpMesh->mTangents[vertexIndex];
				tangent.x = assimpTangent.x;
				tangent.y = assimpTangent.y;
				tangent.z = assimpTangent.z;
			}
			modelMesh->bitangents.push_back(bitangent);
			modelMesh->tangents.push_back(tangent);

			Vec3F color;
			if (assimpMesh->HasVertexColors(vertexIndex))
			{
				const aiColor4D* assimpColor = assimpMesh->mColors[0];
				color.x = assimpColor->r;
				color.x = assimpColor->g;
				color.x = assimpColor->b;
			}
			modelMesh->colors.push_back(color);
		}

		for (unsigned int faceIndicesIndex = 0; faceIndicesIndex < assimpMesh->mNumFaces; ++faceIndicesIndex)
		{
			const aiFace face = assimpMesh->mFaces[faceIndicesIndex];
			if (face.mNumIndices != 3)
			{
				// engine assert since we only should process triangles
			}
			modelMesh->triangles.emplace_back(face.mIndices[0],
											  face.mIndices[1], 
											  face.mIndices[2]);
		}

		if (assimpMesh->HasBones())
		{
			ProcessBones(modelLoadingInfo, assimpMesh);
		}

		const aiMaterial* material = modelLoadingInfo.scene->mMaterials[assimpMesh->mMaterialIndex];
		
		std::vector<ITexture2D*> diffuseTextures;
		for (unsigned int textureIndex = 0; textureIndex < material->GetTextureCount(aiTextureType_DIFFUSE); ++textureIndex)
		{
			// aiString str;
			// material->GetTexture(aiTextureType_DIFFUSE, textureIndex, &str);
			//
			// std::string textureFilePath(str.C_Str());
			// Utility::StandardizeFilePathString(textureFilePath);
			// textureFilePath = modelLoadingInfo.directory + '/' + textureFilePath;
			//
			// diffuseTextures.push_back(AssetManagerLocator::GetAssetManager()->LoadTexture2D(textureFilePath));
		}

		auto& rendererMaterial = modelLoadingInfo.model->renderer->GetMaterial();

		aiColor3D color;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "diffuseColor");
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "specularColor");
		material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "emissiveColor");
		material->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "transparentColor");
		material->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "reflectiveColor");
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		rendererMaterial->SetVec4F(Utility::AssimpColor3DToNightOwlVec4F(color), "ambientColor");

		float materialProperty;
		material->Get(AI_MATKEY_SHININESS, materialProperty);
		rendererMaterial->SetFloat(materialProperty, "shininess");
		material->Get(AI_MATKEY_SHININESS_STRENGTH, materialProperty);
		rendererMaterial->SetFloat(materialProperty, "shininessStrength");
	}

	void AssimpModelLoader::ProcessBones(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{
		std::shared_ptr<Mesh>& modelMesh = modelLoadingInfo.model->renderer->mesh;

		modelMesh->boneWeights.resize(modelMesh->vertices.size());

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

				unsigned int vertexId = weight.mVertexId;
				if (modelMesh->subMeshes.empty() == false)
				{
					vertexId += modelMesh->subMeshes.back().baseVertex;
				}

				BoneWeight* boneWeight = modelMesh->boneWeights.data() + vertexId;

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

	void AssimpModelLoader::ProcessMaterials(ModelLoadingInfo& modelLoadingInfo, const aiMesh* assimpMesh)
	{

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
		int numberOfBones = 1;
		const aiNode* armatureRoot = scene->mMeshes[0]->mBones[0]->mArmature;
		std::stack<const aiNode*> armatureNodesForFindingDepth;
		armatureNodesForFindingDepth.push(armatureRoot);
		while (armatureNodesForFindingDepth.empty() == false)
		{
			const aiNode* armatureNode = armatureNodesForFindingDepth.top();
			armatureNodesForFindingDepth.pop();

			for (unsigned int armatureChildIndex = 0; armatureChildIndex < armatureNode->mNumChildren; ++armatureChildIndex)
			{
				numberOfBones++;

				armatureNodesForFindingDepth.emplace(armatureNode->mChildren[armatureChildIndex]);
			}
		}

		modelLoadingInfo.model->skeleton.resize(numberOfBones);

		std::stack<std::pair<GameObject*, const aiNode*>> armatureNodes;
		armatureNodes.emplace(modelLoadingInfo.model->skeleton.data(), armatureRoot);

		int processedGameObjects = 0;
		while (armatureNodes.empty() == false)
		{
			const std::pair<GameObject*, const aiNode*> armatureNodePair = armatureNodes.top();
			armatureNodes.pop();

			armatureNodePair.first->SetName(armatureNodePair.second->mName.data);
			ENGINE_LOG_INFO("Armature name: {0}", armatureNodePair.second->mName.data);

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

				GameObject* childGameObject = &modelLoadingInfo.model->skeleton.at(processedGameObjects);
				childGameObject->GetTransform()->SetParent(parentGameObjectTransform, false);

				armatureNodes.emplace(childGameObject, armatureNodePair.second->mChildren[armatureChildIndex]);
			}
		}
	}

	void AssimpModelLoader::ProcessAnimation(const aiAnimation* assimpAnimation)
	{
		const std::shared_ptr<Animation> animation = std::make_shared<Animation>();

		animation->SetDuration(assimpAnimation->mDuration);
		animation->SetName(assimpAnimation->mName.data);
		animation->SetTicksPerSecond(assimpAnimation->mTicksPerSecond);

		for (unsigned int channelIndex = 0; channelIndex < assimpAnimation->mNumChannels; ++channelIndex)
		{
			aiNodeAnim* animationChannel = assimpAnimation->mChannels[channelIndex];
			animation->GetBoneKeyFramesMap().emplace(animationChannel->mNodeName.data, BoneKeyFrames(animationChannel));
		}

		AssetManagerLocator::GetAssetManager()->GetAnimationRepository().AddAsset(animation->GetName(), animation);
	}

	bool AssimpModelLoader::HasBones(const ModelLoadingInfo& modelLoadingInfo)
	{
		const aiScene* scene = modelLoadingInfo.scene;
		if (scene == nullptr)
		{
			return false;
		}

		for (unsigned int childIndex = 0; childIndex < scene->mRootNode->mNumChildren; ++childIndex)
		{
			const aiNode* child = scene->mRootNode->mChildren[childIndex];
			if (child->mNumMeshes > 0)
			{
				const unsigned int childMeshIndex = child->mMeshes[0];

				return scene->mMeshes[childMeshIndex]->HasBones();
			}
		}

		return false;
	}
}
