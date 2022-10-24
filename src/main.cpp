#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Component/Materials/IMaterial.h"
#include "NightOwl/Core/Application/Scene.h"
#include "NightOwl/Core/Time/Time.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"
#include "NightOwl/Input/Input.h"
#include "NightOwl/Physics/2D/PhysicsEngine2D.h"
#include "NightOwl/Core/Locator/PhysicsEngine2DLocator.h"
#include "NightOwl/Physics/2D/Collider/AABBBoxCollider2D.h"
#include "NightOwl/Physics/2D/Collider/CircleCollider2D.h"

int main()
{
	// Standup necessary systems
	NightOwl::Utility::LoggerManager::Init();
	NightOwl::Window::WindowApi::CreateWindow("Night Owl Engine Demo", 600, 800);
	NightOwl::Input::Input::Init();
	NightOwl::Core::Time::Init();
	NightOwl::Physics::PhysicsEngine2D engine;
	NightOwl::Core::PhysicsEngine2DLocator::Provide(&engine);

	// Create textures
	const auto ofMonstersAndMenTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/Of_Monsters_And_Men.jpg");
	const auto theLastOfUsTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/The_Last_Of_Us.jpg");

	// Create scene for game objects
	NightOwl::Core::Scene scene;

	// Create mesh data for game object meshes (Will be moved out of main in next iteration)
	const std::vector smallQuad = {
		 NightOwl::Math::Vec3F(0.5f,  0.5f, 0.0f),
		 NightOwl::Math::Vec3F(0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f,  0.5f, 0.0f)
	};

	const std::vector largerQuad = {
		 NightOwl::Math::Vec3F(0.75f,  0.5f, 0.0f),
		 NightOwl::Math::Vec3F(0.75f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.75f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.75f,  0.5f, 0.0f)
	};

	const std::vector colors = {
		NightOwl::Math::Vec3F(1.0f,  0.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  0.0f, 1.0f),
		NightOwl::Math::Vec3F(1.0f,  0.0f, 1.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f)
	};

	const std::vector triangles = {
		NightOwl::Math::Vec3UI(0,  1, 3),  // first Triangle
		NightOwl::Math::Vec3UI(1,  2, 3)  // second Triangle
	};

	const std::vector uvs = {
		 NightOwl::Math::Vec2F(1.0f,  1.0f),
		 NightOwl::Math::Vec2F(1.0f, 0.0f),
		 NightOwl::Math::Vec2F(0.0f, 0.0f),
		 NightOwl::Math::Vec2F(0.0f,  1.0f)
	};


	NightOwl::Math::QuatF quat(NightOwl::Math::Vec3F::Right(), 90);
	quat *= NightOwl::Math::QuatF::MakeRotationY(90);
	quat.Normalize();
	quat.GetEulerAngles();
	quat *= NightOwl::Math::QuatF::MakeRotationZ(90);
	quat.Normalize();
	quat.GetEulerAngles();
	//Add camera to scene
	NightOwl::GameObject::GameObject& camera = scene.AddGameObject("Camera");
	NightOwl::Component::Camera* cameraComponent = camera.AddComponent<NightOwl::Component::Camera>();
	camera.GetTransform()->Translate(0, 0, -10, NightOwl::Component::Space::World);

	// Add quads to scene with corresponding mesh data
	NightOwl::GameObject::GameObject& quad = scene.AddGameObject("Cube");
	auto quadMeshRenderer = quad.AddComponent<NightOwl::Component::MeshRenderer>();
	quadMeshRenderer->GetMaterial()->SetTexture(ofMonstersAndMenTexture);
	auto mesh = quadMeshRenderer->GetMesh();
	mesh->SetVertices(smallQuad);
	mesh->SetColors(colors);
	mesh->SetUVs(uvs);
	mesh->SetTriangles(triangles);
	auto quadRigidBody2D = quad.AddComponent<NightOwl::Component::RigidBody2D>();
	quadRigidBody2D->SetMass(1.0f);
	NightOwl::Physics::Collider2D* aabbCollider = new NightOwl::Physics::AABBBoxCollider2D(NightOwl::Math::Vec2F(0, 0), NightOwl::Math::Vec2F(0.5, 0.5));
	quadRigidBody2D->SetCollider(aabbCollider);
	
	NightOwl::GameObject::GameObject& quad2 = scene.AddGameObject("Cube2");
	auto cube2MeshRenderer = quad2.AddComponent<NightOwl::Component::MeshRenderer>();
	cube2MeshRenderer->GetMaterial()->SetTexture(theLastOfUsTexture);
	mesh = cube2MeshRenderer->GetMesh();
	mesh->SetVertices(largerQuad);
	mesh->SetColors(colors);
	mesh->SetUVs(uvs);
	mesh->SetTriangles(triangles);


	NightOwl::GameObject::GameObject& quad3 = scene.AddGameObject("Cube3");
	auto quad3MeshRenderer = quad3.AddComponent<NightOwl::Component::MeshRenderer>();
	mesh = quad3MeshRenderer->GetMesh();
	mesh->SetVertices(smallQuad);
	mesh->SetColors(colors);
	mesh->SetTriangles(triangles);
	auto quad3RigidBody2D = quad3.AddComponent<NightOwl::Component::RigidBody2D>();
	quad3RigidBody2D->SetMass(1.0f);
	aabbCollider = new NightOwl::Physics::CircleCollider2D(NightOwl::Math::Vec2F(0, 0), 2.5);
	quad3RigidBody2D->SetCollider(aabbCollider);

	// Position objects in the scene
	quad.GetTransform()->SetParent(quad2.GetTransform());
	quad.GetTransform()->Scale(3, 3, 3, NightOwl::Component::Space::Local);
	quad3.GetTransform()->SetParent(quad.GetTransform());

	camera.GetTransform()->Translate(0, 0, 20, NightOwl::Component::Space::Local);

	quad.GetTransform()->Translate(4.5, 0, 0, NightOwl::Component::Space::Local);
	quad3.GetTransform()->Translate(4, 0, 0, NightOwl::Component::Space::Local);

	const NightOwl::Math::Vec4F clearColor(0.2f, 0.3f, 0.3f, 1.0f);
	while(!NightOwl::Window::WindowApi::GetWindow()->ShouldWindowClose())
	{
		NightOwl::Graphics::RenderApi::GetContext()->ClearColor(clearColor);
		NightOwl::Graphics::RenderApi::GetContext()->ClearBuffer();

		if(NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyUp))
		{
			//quad3.GetTransform()->Translate(-0.01, 0, 0, NightOwl::Component::Space::Local);
			quad3RigidBody2D->AddForce(NightOwl::Math::Vec2F::Up() * 1000.0f);
		}

		if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyDown))
		{
			//quad3.GetTransform()->Translate(0.01, 0, 0, NightOwl::Component::Space::Local);
			quad3RigidBody2D->AddForce(NightOwl::Math::Vec2F::Down() * 1000.0f);
		}

		if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyRight))
		{
			//quad3.GetTransform()->Translate(0.01, 0, 0, NightOwl::Component::Space::Local);
			quad3RigidBody2D->AddForce(NightOwl::Math::Vec2F::Right() * 1000.0f);
		}

		if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyLeft))
		{
			//quad3.GetTransform()->Translate(0.01, 0, 0, NightOwl::Component::Space::Local);
			quad3RigidBody2D->AddForce(NightOwl::Math::Vec2F::Left() * 1000.0f);
		}

		if (NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeyP))
		{
			quad3.GetTransform()->SetParent(quad.GetTransform());
		}

		if (NightOwl::Input::Input::IsKeyHeld(NightOwl::Input::KeyCode::KeyU))
		{
			quad3.GetTransform()->RemoveParent();
		}

		if (NightOwl::Input::Input::IsKeyPressed(NightOwl::Input::KeyCode::KeySpace))
		{
			quad3.GetTransform()->SetRotation(NightOwl::Math::QuatF::MakeRotationY(0));
		}

		// Update scene graph
		scene.Update();
		engine.Update();

		NightOwl::Input::Input::Update();

		quadMeshRenderer->Draw();
		cube2MeshRenderer->Draw();
		quad3MeshRenderer->Draw();

		NightOwl::Window::WindowApi::GetWindow()->Update();
		NightOwl::Core::Time::Update();
	}
		
	return 0;
}
