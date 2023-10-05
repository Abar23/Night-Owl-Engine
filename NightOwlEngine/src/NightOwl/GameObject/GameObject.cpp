#include <NightOwlPch.h>

#include "GameObject.h"
#include "NightOwl/Behavior/OwlBehaviorManager.h"
#include "NightOwl/Core/Application/Scene.h"
#include <limits>

namespace NightOwl
{
	GameObject::GameObject()
		:	scene(nullptr),
			id(std::numeric_limits<unsigned int>::max()),
			isActive(false),
			shouldDestroy(false)
	{
		transform.gameObject = this;
	}

	GameObject::GameObject(Scene* scene, unsigned id)
		:	scene(scene),
			id(id),
			isActive(true),
			shouldDestroy(false)
	{
		transform.gameObject = this;
	}

	GameObject::GameObject(const std::string& name, Scene* scene, unsigned id)
		:	scene(scene),
			name(name),
			id(id),
			isActive(true),
			shouldDestroy(false)
	{
		transform.gameObject = this;
	}

	bool GameObject::IsActive() const
	{
		return isActive;
	}

	void GameObject::SetActive(bool active)
	{
		if (this->isActive != active)
		{
			this->isActive = active;

			for (auto& element : componentList)
			{
				element.get()->isEnabled = this->isActive;
			}
		}
	}

	const std::string& GameObject::GetName()
	{
		return name;
	}

	void GameObject::SetName(const std::string& name)
	{
		this->name = name;
	}

	void GameObject::SetScene(Scene* scene)
	{
		this->scene = scene;

		if (transform.HasChildren() == false)
		{
			return;
		}

		for (unsigned int childrenIndex = 0; childrenIndex < transform.GetNumberOfChildren(); ++childrenIndex)
		{
			transform.GetChildAtIndex(childrenIndex)->gameObject->SetScene(scene);
		}
	}

	Transform* GameObject::GetTransform()
	{
		return &transform;
	}

	unsigned GameObject::GetId() const
	{
		return id;
	}

	Scene* GameObject::GetScene()
	{
		return scene;
	}

	std::shared_ptr<GameObject> GameObject::Clone(Scene* currentScene /* = nullptr */)
	{
		Scene* activeScene = currentScene ? currentScene : scene;
		auto clonedGameObject = std::make_shared<GameObject>();
		clonedGameObject->scene = activeScene;
		clonedGameObject->name = name;
		clonedGameObject->isActive = true;
		clonedGameObject->transform.Clone(transform, activeScene);
		clonedGameObject->transform.gameObject = clonedGameObject.get();

		for (const auto& component : componentList)
		{
			std::shared_ptr<Component> clonedComponent = component->Clone();

			if (clonedComponent == nullptr) continue;

			clonedComponent->gameObject = clonedGameObject.get();
			clonedGameObject->componentList.push_back(clonedComponent);

			OwlBehavior* owlBehavior = dynamic_cast<OwlBehavior*>(clonedComponent.get());
			if (owlBehavior)
			{
				clonedGameObject->owlBehaviorList.insert(owlBehavior);
				OwlBehaviorManagerLocator::GetOwlBehaviorManager()->AddOwlBehavior(owlBehavior);
			}
		}

		if(activeScene != nullptr)
		{
			activeScene->AddClone(clonedGameObject);
		}

		return clonedGameObject;
	}

	void GameObject::Destroy()
	{
		this->shouldDestroy = true;
		this->SetActive(false);
	}

	const std::set<OwlBehavior*>& GameObject::GetOwlBehaviorList()
	{
		return owlBehaviorList;
	}

	void GameObject::DestroyGameObject()
	{
		for (auto& component : componentList)
		{
			component->Remove();
		}

		for (int childIndex = 0; childIndex < transform.GetNumberOfChildren(); ++childIndex)
		{
			transform.GetChildAtIndex(childIndex)->GetGameObject().DestroyGameObject();
		}

		if(scene != nullptr)
		{
			scene->RemoveGameObject(*this);
		}
	}
}
