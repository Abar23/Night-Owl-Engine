#include "SceneSerializer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include <cstdio>

namespace NightOwl::Core
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

	void SceneSerializer::SerializeGameObject(GameObject::GameObject& gameObject, rapidjson::Document& document, rapidjson::Value& gameObjectList)
	{
		rapidjson::Value gameObjectJsonObjKey(gameObject.GetName().c_str(), document.GetAllocator());
		rapidjson::Value gameObjectJsonObj(rapidjson::kObjectType);

		Component::Transform::classDescription.Serialize(gameObject.GetTransform(), document, gameObjectJsonObj);

		for (auto& component : gameObject.componentList)
		{
			if(dynamic_cast<Component::Camera*>(component.get()))
			{
				const Component::Camera* camera = dynamic_cast<Component::Camera*>(component.get());
				Component::Camera::classDescription.Serialize(camera, document, gameObjectJsonObj);
			}
			else if (dynamic_cast<Component::RigidBody2D*>(component.get()))
			{
				const Component::RigidBody2D* rigidBody2D = dynamic_cast<Component::RigidBody2D*>(component.get());
				Component::RigidBody2D::classDescription.Serialize(rigidBody2D, document, gameObjectJsonObj);
			}
			else if (dynamic_cast<Component::MeshRenderer*>(component.get()))
			{
				const Component::MeshRenderer* meshRenderer = dynamic_cast<Component::MeshRenderer*>(component.get());
				Component::MeshRenderer::classDescription.Serialize(meshRenderer, document, gameObjectJsonObj);
			}
		}

		gameObjectList.AddMember(gameObjectJsonObjKey, gameObjectJsonObj, document.GetAllocator());
	}
}

