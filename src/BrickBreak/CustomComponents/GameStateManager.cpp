#include "GameStateManager.h"
#include "BottomWall.h"
#include "BrickManager.h"
#include "PaddleController.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"

namespace NightOwl
{
	namespace Component
	{
		class MeshRenderer;
	}
}

namespace BrickBreak
{
	void GameStateManager::Awake()
	{
		needToSetBallPosition = true;
		playerHasWon = false;
		numberOfLives = 3;
	}

	void GameStateManager::Start()
	{
		ball = gameObject->GetScene()->FindGameObject("Ball");
		paddle = gameObject->GetScene()->FindGameObject("Paddle");
		bottomWall = gameObject->GetScene()->FindGameObject("Bottom Wall");

		youWin = gameObject->GetScene()->FindGameObject("You Win");
		youLose = gameObject->GetScene()->FindGameObject("You Lose");

		youWin->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(false);
		youLose->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(false);

		// TODO: Need to fix the method below!!!!!!!!!!!
		//paddle->GetTransform()->SetChild(*ball->GetTransform());
	}

	void GameStateManager::Update()
	{
		if (numberOfLives > 0)
		{
			if (needToSetBallPosition)
			{
				NightOwl::Math::Vec3F paddlePosition = paddle->GetTransform()->GetPosition();
				ball->GetTransform()->SetPosition({ paddlePosition.x + (PADDLE_HALF_WIDTH / 2), paddlePosition.y + PADDLE_HALF_HEIGHT + BALL_HALF_HEIGHT, 0.0f });
				ball->GetTransform()->SetParent(paddle->GetTransform());
				ball->GetComponent<NightOwl::Component::RigidBody2D>()->SetVelocity({ 0.0f, 0.0f });
				needToSetBallPosition = false;
			}

			if (bottomWall->GetComponent<BottomWall>()->HasCollidedWithBall())
			{
				bottomWall->GetComponent<BottomWall>()->SetHasCollidedWithBall(false);
				numberOfLives--;
				needToSetBallPosition = true;
			}
		}

		if(numberOfLives == 0 && playerHasWon == false)
		{
			if(!youLose->GetComponent<NightOwl::Component::MeshRenderer>()->IsVisible())
			{
				youLose->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(true);
			}

			if(NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeyR))
			{
				youLose->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(false);
				numberOfLives = 3;

				NightOwl::Math::Vec3F paddlePosition = paddle->GetTransform()->GetPosition();

				paddlePosition.x = 0;
				paddle->GetTransform()->SetPosition(paddlePosition);

				ball->GetTransform()->SetPosition({ paddlePosition.x + (PADDLE_HALF_WIDTH / 2), paddlePosition.y + PADDLE_HALF_HEIGHT + BALL_HALF_HEIGHT, 0.0f });
				ball->GetTransform()->SetParent(paddle->GetTransform());
				ball->GetComponent<NightOwl::Component::RigidBody2D>()->SetVelocity({ 0.0f, 0.0f });

				gameObject->GetScene()->FindGameObject("Brick Manager")->GetComponent<BrickManager>()->TurnOnBricks();
			}
		}

		if (playerHasWon)
		{
			if (!youWin->GetComponent<NightOwl::Component::MeshRenderer>()->IsVisible())
			{
				youWin->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(true);
			}

			if (NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeyR))
			{
				youWin->GetComponent<NightOwl::Component::MeshRenderer>()->SetVisible(false);
				numberOfLives = 3;

				NightOwl::Math::Vec3F paddlePosition = paddle->GetTransform()->GetPosition();

				paddlePosition.x = 0;
				paddle->GetTransform()->SetPosition(paddlePosition);

				ball->GetTransform()->SetPosition({ paddlePosition.x + (PADDLE_HALF_WIDTH / 2), paddlePosition.y + PADDLE_HALF_HEIGHT + BALL_HALF_HEIGHT, 0.0f });
				ball->GetTransform()->SetParent(paddle->GetTransform());
				ball->GetComponent<NightOwl::Component::RigidBody2D>()->SetVelocity({ 0.0f, 0.0f });

				gameObject->GetScene()->FindGameObject("Brick Manager")->GetComponent<BrickManager>()->TurnOnBricks();

				playerHasWon = false;
			}
		}
	}

	void GameStateManager::OnEnable()
	{

	}

	void GameStateManager::OnDisable()
	{

	}

	void GameStateManager::OnCollisionEnter2D(const NightOwl::Physics::Collision2D& collision2D)
	{

	}

	void GameStateManager::PlayerWins()
	{
		playerHasWon = true;
	}
}
