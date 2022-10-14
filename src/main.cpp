#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/MeshRenderer.h"
#include "NightOwl/Component/Concrete/Transform.h"
#include "NightOwl/Component/Materials/IMaterial.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/WindowApi.h"
#include "NightOwl/GameObject/GameObject.h"

int main()
{
	NightOwl::Math::Vec3F a(1, 2, 3);
	NightOwl::Utility::LoggerManager::Init();

	NightOwl::Window::WindowApi::CreateWindow("Hi", 600, 800);

	auto ofMonstersAndMenTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/Of_Monsters_And_Men.jpg");
	auto theLastOfUsTexture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/The_Last_Of_Us.jpg");

	const std::vector vertices = {
		 NightOwl::Math::Vec3F(0.5f,  0.5f, 0.0f),
		 NightOwl::Math::Vec3F(0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f,  0.5f, 0.0f)
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

	NightOwl::GameObject::GameObject camera("Camera");
	camera.AddComponent<NightOwl::Component::Camera>();
	camera.GetTransform()->Translate(0, 0, -3.5, NightOwl::Component::Space::Local);

	NightOwl::GameObject::GameObject cube("Cube");
	auto cubeMeshRenderer = cube.AddComponent<NightOwl::Component::MeshRenderer>();
	cubeMeshRenderer->GetMaterial()->SetTexture(ofMonstersAndMenTexture);
	auto mesh = cubeMeshRenderer->GetMesh();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetUVs(uvs);
	mesh->SetTriangles(triangles);

	NightOwl::GameObject::GameObject cube2("Cube2");
	auto cube2MeshRenderer = cube2.AddComponent<NightOwl::Component::MeshRenderer>();
	cube2MeshRenderer->GetMaterial()->SetTexture(theLastOfUsTexture);
	mesh = cube2MeshRenderer->GetMesh();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetUVs(uvs);
	mesh->SetTriangles(triangles);


	NightOwl::GameObject::GameObject cube3("Cube3");
	auto cube3MeshRenderer = cube3.AddComponent<NightOwl::Component::MeshRenderer>();
	mesh = cube3MeshRenderer->GetMesh();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetTriangles(triangles);

	cube.GetTransform()->SetParent(*cube2.GetTransform());
	cube3.GetTransform()->SetParent(*cube.GetTransform());

	cube.GetTransform()->Translate(2.5, 0, 0, NightOwl::Component::Space::Local);
	cube.GetTransform()->Scale(1, 1, 1, NightOwl::Component::Space::World);
	cube3.GetTransform()->Translate(2, 0, 0, NightOwl::Component::Space::Local);

	NightOwl::Math::Vec4F color(0.2f, 0.3f, 0.3f, 1.0f);
	while(!NightOwl::Window::WindowApi::GetWindow()->ShouldWindowClose())
	{
		cube.GetTransform()->Rotate(0, 0, 1, NightOwl::Component::Space::Local);
		cube2.GetTransform()->Rotate(0, 0, 1, NightOwl::Component::Space::Local);
		cube3.GetTransform()->Rotate(0, 0, 1, NightOwl::Component::Space::Local);

		NightOwl::Graphics::RenderApi::GetContext()->ClearColor(color);
		NightOwl::Graphics::RenderApi::GetContext()->ClearBuffer();

		cubeMeshRenderer->Draw();
		cube2MeshRenderer->Draw();
		cube3MeshRenderer->Draw();

		NightOwl::Window::WindowApi::GetWindow()->Update();
	}
		
	return 0;
}
