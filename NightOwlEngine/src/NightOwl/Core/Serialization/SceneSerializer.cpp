#include "NightOwlPch.h"

#include "SceneSerializer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include <cstdio>

namespace NightOwl
{
	SceneSerializer::SceneSerializer(Scene& scene)
		:	scene(&scene)
	{ }

	void SceneSerializer::Serialize(const std::string& fileName)
	{
		FILE* file;

		errno_t error = fopen_s(&file, fileName.c_str(), "wb");

		if(error == 0)
		{
			rapidjson::Document document;

			document.SetObject();

			rapidjson::Value sceneKey("Scene", document.GetAllocator());
			rapidjson::Value sceneJsonObj(rapidjson::kObjectType);

			rapidjson::Value gameObjectKey("Game Objects", document.GetAllocator());
			rapidjson::Value gameObjectsJsonObj(rapidjson::kObjectType);
			for (auto& idToGameObjectPair : scene->gameObjectsList)
			{
				SerializeGameObject(*idToGameObjectPair.second, document, gameObjectsJsonObj);
			}
			sceneJsonObj.AddMember(gameObjectKey, gameObjectsJsonObj, document.GetAllocator());

			document.AddMember(sceneKey, sceneJsonObj, document.GetAllocator());

			char writeBuffer[65536];
			rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
			rapidjson::PrettyWriter writer(os);
			document.Accept(writer);

			fclose(file);
		}
	}

	void SceneSerializer::Deserialize(const std::string& fileName)
	{
	}

	void SceneSerializer::SerializeGameObject(GameObject& gameObject, rapidjson::Document& document, rapidjson::Value& gameObjectList)
	{
		rapidjson::Value gameObjectJsonObjKey(gameObject.GetName().c_str(), document.GetAllocator());
		rapidjson::Value gameObjectJsonObj(rapidjson::kObjectType);

		Transform::classDescription.Serialize(gameObject.GetTransform(), document, gameObjectJsonObj);

		for (auto& component : gameObject.componentList)
		{
			if(dynamic_cast<Camera*>(component.get()))
			{
				const Camera* camera = dynamic_cast<Camera*>(component.get());
				Camera::classDescription.Serialize(camera, document, gameObjectJsonObj);
			}
			else if (dynamic_cast<RigidBody2D*>(component.get()))
			{
				const RigidBody2D* rigidBody2D = dynamic_cast<RigidBody2D*>(component.get());
				RigidBody2D::classDescription.Serialize(rigidBody2D, document, gameObjectJsonObj);
			}
			else if (dynamic_cast<MeshRenderer*>(component.get()))
			{
				const MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(component.get());
				//MeshRenderer::classDescription.Serialize(meshRenderer, document, gameObjectJsonObj);
			}
		}

		gameObjectList.AddMember(gameObjectJsonObjKey, gameObjectJsonObj, document.GetAllocator());
	}
}

