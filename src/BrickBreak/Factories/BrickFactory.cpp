#include "BrickFactory.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "BrickBreak/CustomComponents/Brick.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Materials/IMaterial.h"
#include "NightOwl/Physics/2D/Collider/AABBBoxCollider2D.h"

namespace BrickBreak
{
	BrickFactory::BrickFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas)
		:	scene(brickScene),
			textureAtlas(textureAtlas)
	{
	}

	NightOwl::GameObject::GameObject& BrickFactory::CreateBrick(BrickType type) const
	{
		NightOwl::Physics::Collider2D* brickHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { BRICK_HALF_WIDTH - 0.1f, BRICK_HALF_HEIGHT - 0.1f });

		switch (type)
		{
			case BrickType::RED:
			{
				auto& brick = scene.AddGameObject("Red Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(RED_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}

			case BrickType::BLUE:
			{
				auto& brick = scene.AddGameObject("Blue Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(BLUE_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}

			case BrickType::GREEN:
			{
				auto& brick = scene.AddGameObject("Green Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(GREEN_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}

			case BrickType::PURPLE:
			{
				auto& brick = scene.AddGameObject("Purple Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(PURPLE_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}

			case BrickType::YELLOW:
			{
				auto& brick = scene.AddGameObject("Yellow Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(YELLOW_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}

			case BrickType::ORANGE:
			default:
			{
				auto& brick = scene.AddGameObject("Orange Brick");

				auto* brickRender = brick.AddComponent<NightOwl::Component::MeshRenderer>();
				brickRender->GetMaterial()->SetTexture(textureAtlas);

				auto* brickMesh = brickRender->GetMesh();
				brickMesh->SetVertices(BRICK_VERTICES);
				brickMesh->SetUVs(ORANGE_BRICK_UVS);
				brickMesh->SetTriangles(QUAD_TRIANGLES);

				auto* brickRigidBody = brick.AddComponent<NightOwl::Component::RigidBody2D>();
				brickRigidBody->SetCollider(brickHitBox);

				brick.AddComponent<Brick>();

				return brick;
			}
		}
	}
}
