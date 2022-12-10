#include "PaddleAndBallFactory.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "BrickBreak/CustomComponents/Ball.h"
#include "BrickBreak/CustomComponents/PaddleController.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Materials/IMaterial.h"
#include "NightOwl/Physics/2D/Collider/AABBBoxCollider2D.h"
#include "NightOwl/Physics/2D/Collider/CircleCollider2D.h"

namespace BrickBreak
{
	PaddleAndBallFactory::PaddleAndBallFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas)
		:	scene(brickScene),
			textureAtlas(textureAtlas)
	{ }

	NightOwl::GameObject::GameObject& PaddleAndBallFactory::CreateBall() const
	{
		auto& ball = scene.AddGameObject("Ball");

		auto* ballRenderer = ball.AddComponent<NightOwl::Component::MeshRenderer>();
		ballRenderer->GetMaterial()->SetTexture(textureAtlas);

		auto* ballMesh = ballRenderer->GetMesh();
		ballMesh->SetVertices(BALL_VERTICES);
		ballMesh->SetUVs(BALL_UVS);
		ballMesh->SetTriangles(QUAD_TRIANGLES);

		NightOwl::Physics::Collider2D* ballHitBox = new NightOwl::Physics::CircleCollider2D({ 0, 0 }, BALL_HALF_WIDTH);
		auto* ballRigidBody = ball.AddComponent<NightOwl::Component::RigidBody2D>();
		ballRigidBody->SetCollider(ballHitBox);

		ball.AddComponent<Ball>();

		return ball;
	}

	NightOwl::GameObject::GameObject& PaddleAndBallFactory::CreatePaddle() const
	{
		auto& paddle = scene.AddGameObject("Paddle");

		auto* paddleRenderer = paddle.AddComponent<NightOwl::Component::MeshRenderer>();
		paddleRenderer->GetMaterial()->SetTexture(textureAtlas);

		auto* paddleMesh = paddleRenderer->GetMesh();
		paddleMesh->SetVertices(PADDLE_VERTICES);
		paddleMesh->SetUVs(PADDLE_UVS);
		paddleMesh->SetTriangles(QUAD_TRIANGLES);

		NightOwl::Physics::Collider2D* ballHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { PADDLE_HALF_WIDTH, PADDLE_HALF_HEIGHT });
		auto* paddleRigidBody = paddle.AddComponent<NightOwl::Component::RigidBody2D>();
		paddleRigidBody->SetCollider(ballHitBox);

		paddle.AddComponent<PaddleController>();

		return paddle;
	}
}
