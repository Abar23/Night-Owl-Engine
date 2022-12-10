#include "WallFactory.h"
#include "BrickBreak/BrickBreakData/Constants.h"
#include "BrickBreak/CustomComponents/BottomWall.h"
#include "BrickBreak/CustomComponents/SideWall.h"
#include "BrickBreak/CustomComponents/TopWall.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/RigidBody2D.h"
#include "NightOwl/Component/Materials/IMaterial.h"
#include "NightOwl/Physics/2D/Collider/AABBBoxCollider2D.h"

namespace BrickBreak
{
	WallFactory::WallFactory(NightOwl::Core::Scene& brickScene, const std::shared_ptr<NightOwl::Graphics::ITexture2D>& textureAtlas)
		:	scene(brickScene),
			textureAtlas(textureAtlas)
	{ }

	NightOwl::GameObject::GameObject& WallFactory::CreateWall(WallType type) const
	{
		switch (type)
		{
			case WallType::LeftWall:
			{
				auto& wall = scene.AddGameObject("Left Wall");

				auto* wallRender = wall.AddComponent<NightOwl::Component::MeshRenderer>();
				wallRender->GetMaterial()->SetTexture(textureAtlas);

				auto* wallMesh = wallRender->GetMesh();
				wallMesh->SetVertices(WALL_VERTICES);
				wallMesh->SetUVs(LEFT_WALL_UVS);
				wallMesh->SetTriangles(QUAD_TRIANGLES);

				auto* wallRigidBody = wall.AddComponent<NightOwl::Component::RigidBody2D>();
				NightOwl::Physics::Collider2D* wallHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { WALL_HALF_WIDTH, WALL_HALF_HEIGHT });
				wallRigidBody->SetCollider(wallHitBox);

				wall.AddComponent<SideWall>();

				return wall;
			}

			case WallType::RightWall:
			{
				auto& wall = scene.AddGameObject("Right Wall");

				auto* wallRender = wall.AddComponent<NightOwl::Component::MeshRenderer>();
				wallRender->GetMaterial()->SetTexture(textureAtlas);

				auto* wallMesh = wallRender->GetMesh();
				wallMesh->SetVertices(WALL_VERTICES);
				wallMesh->SetUVs(RIGHT_WALL_UVS);
				wallMesh->SetTriangles(QUAD_TRIANGLES);

				auto* wallRigidBody = wall.AddComponent<NightOwl::Component::RigidBody2D>();
				NightOwl::Physics::Collider2D* wallHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { WALL_HALF_WIDTH, WALL_HALF_HEIGHT });
				wallRigidBody->SetCollider(wallHitBox);

				wall.AddComponent<SideWall>();

				return wall;
			}

			case WallType::TopWall:
			{
				auto& wall = scene.AddGameObject("Top Wall");

				wall.GetTransform()->Translate(0.0f, 127.75f, 0.0f, NightOwl::Component::Space::World);

				auto* wallRigidBody = wall.AddComponent<NightOwl::Component::RigidBody2D>();
				NightOwl::Physics::Collider2D* wallHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { 168, 2 });
				wallRigidBody->SetCollider(wallHitBox);

				wall.AddComponent<TopWall>();

				return wall;
			}

			case WallType::BottomWall:
			default:
			{
				auto& wall = scene.AddGameObject("Bottom Wall");

				wall.GetTransform()->Translate(0.0f, -135.75f, 0.0f, NightOwl::Component::Space::World);

				auto* wallRigidBody = wall.AddComponent<NightOwl::Component::RigidBody2D>();
				NightOwl::Physics::Collider2D* wallHitBox = new NightOwl::Physics::AABBBoxCollider2D({ 0, 0 }, { 168, 2 });
				wallRigidBody->SetCollider(wallHitBox);

				wall.AddComponent<BottomWall>();

				return wall;
			}
		}
	}
}
