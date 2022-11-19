#pragma once

#include "NightOwl/Core/Application/Scene.h"
#include "rapidjson/document.h"

namespace NightOwl::Core
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Scene& scene);

		void Serialize(const std::string& fileName);

		void Deserialize(const std::string& fileName);

	private:
		Scene* scene;

		void SerializeGameObject(GameObject::GameObject& gameObject, rapidjson::Document& document, rapidjson::Value& gameObjectList);
	};
}
