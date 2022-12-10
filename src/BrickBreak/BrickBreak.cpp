#include "BrickBreak.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "CustomComponents/BrickManager.h"
#include "CustomComponents/GameStateManager.h"
#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "Factories/BrickFactory.h"
#include "Factories/PaddleAndBallFactory.h"
#include "Factories/WallFactory.h"
#include "Factories/YouWinLoseFactory.h"

namespace BrickBreak
{
	void BrickBreak::Init()
	{
		//Add camera to scene
		NightOwl::GameObject::GameObject& camera = scene.AddGameObject("Camera");
		NightOwl::Component::Camera* cameraComponent = camera.AddComponent<NightOwl::Component::Camera>();
		cameraComponent->SetPerspectiveMode(false);
		cameraComponent->SetOrthographicSize(126.75f);
		camera.GetTransform()->Translate(0, 0, 1, NightOwl::Component::Space::World);

		const auto textureAtlas = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/Brick_Break_Atlas.png");
		const auto youWinTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/You Win.png");
		const auto youLoseTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/You Lose.png");

		BrickFactory brickFactory(scene, textureAtlas);
		PaddleAndBallFactory ballAndPaddleFactory(scene, textureAtlas);
		WallFactory wallFactory(scene, textureAtlas);
		YouWinLoseFactory youWinLoseFactory(scene, youWinTexture, youLoseTexture);

		ballAndPaddleFactory.CreateBall();
		auto& paddle = ballAndPaddleFactory.CreatePaddle();
		paddle.GetTransform()->Translate(0, -112.5 + PADDLE_HALF_HEIGHT, 0, NightOwl::Component::Space::World);

		for(int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::RED);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT, 0, NightOwl::Component::Space::World);
		}

		for (int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::GREEN);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT * 3, 0, NightOwl::Component::Space::World);
		}

		for (int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::YELLOW);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT * 5, 0, NightOwl::Component::Space::World);
		}

		for (int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::BLUE);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT * 7, 0, NightOwl::Component::Space::World);
		}

		for (int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::ORANGE);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT * 9, 0, NightOwl::Component::Space::World);
		}

		for (int i = 0; i < 10; ++i)
		{
			NightOwl::GameObject::GameObject& brick = brickFactory.CreateBrick(BrickType::PURPLE);
			brick.GetTransform()->Translate(i * BRICK_WIDTH - 135.0f, 112.5 - BRICK_HALF_HEIGHT * 11, 0, NightOwl::Component::Space::World);
		}

		auto& rightWall1 = wallFactory.CreateWall(WallType::RightWall);
		rightWall1.GetTransform()->Translate(169 - WALL_HALF_WIDTH, WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& rightWall2 = wallFactory.CreateWall(WallType::RightWall);
		rightWall2.GetTransform()->Translate(169 - WALL_HALF_WIDTH, WALL_HALF_HEIGHT + WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& rightWall3 = wallFactory.CreateWall(WallType::RightWall);
		rightWall3.GetTransform()->Translate(169 - WALL_HALF_WIDTH, -WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& rightWall4 = wallFactory.CreateWall(WallType::RightWall);
		rightWall4.GetTransform()->Translate(169 - WALL_HALF_WIDTH, -WALL_HALF_HEIGHT - WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);

		auto& leftWall1 = wallFactory.CreateWall(WallType::LeftWall);
		leftWall1.GetTransform()->Translate(-169 + WALL_HALF_WIDTH, WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& leftWall2 = wallFactory.CreateWall(WallType::LeftWall);
		leftWall2.GetTransform()->Translate(-169 + WALL_HALF_WIDTH, WALL_HALF_HEIGHT + WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& leftWall3 = wallFactory.CreateWall(WallType::LeftWall);
		leftWall3.GetTransform()->Translate(-169 + WALL_HALF_WIDTH, -WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);
		auto& leftWall4 = wallFactory.CreateWall(WallType::LeftWall);
		leftWall4.GetTransform()->Translate(-169 + WALL_HALF_WIDTH, -WALL_HALF_HEIGHT - WALL_HALF_HEIGHT, 0.0, NightOwl::Component::Space::World);

		wallFactory.CreateWall(WallType::TopWall);
		wallFactory.CreateWall(WallType::BottomWall);

		auto& gameStateManager = scene.AddGameObject("Game State Manager");
		gameStateManager.AddComponent<GameStateManager>();

		auto& brickManager = scene.AddGameObject("Brick Manager");
		brickManager.AddComponent<BrickManager>();

		youWinLoseFactory.CreateLose();
		youWinLoseFactory.CreateWin();
	}

	void BrickBreak::Run()
	{
		scene.Update();
	}
}
