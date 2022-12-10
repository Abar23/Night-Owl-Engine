#include "BrickManager.h"

#include "Brick.h"
#include "GameStateManager.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/GameObject/GameObject.h"

namespace BrickBreak
{
	void BrickManager::Awake()
	{
		numberOfBricks = 0;
		numberOfActiveBricks = 0;
	}

	void BrickManager::Start()
	{
		
	}

	void BrickManager::Update()
	{
		if(numberOfActiveBricks == 0)
		{
			gameObject->GetScene()->FindGameObject("Game State Manager")->GetComponent<GameStateManager>()->PlayerWins();
		}
	}

	void BrickManager::TurnOnBricks()
	{
		for(int i = 0; i < numberOfBricks; i++)
		{
			auto& brick = gameObject->GetTransform()->GetChildAtIndex(i)->GetGameObject();

			brick.GetComponent<Brick>()->ReactivateBrick();
		}

		numberOfActiveBricks = numberOfBricks;
	}

	void BrickManager::RegisterBrick(NightOwl::GameObject::GameObject* brick)
	{
		numberOfBricks++;
		numberOfActiveBricks++;

		brick->GetTransform()->SetParent(gameObject->GetTransform());
	}

	void BrickManager::DecreaseNumberOfActiveBricks()
	{
		numberOfActiveBricks--;
	}
}
