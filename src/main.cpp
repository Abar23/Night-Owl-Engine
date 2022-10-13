#include "NightOwl/Component/Concrete/Camera.h"
#include "NightOwl/Component/Concrete/Mesh.h"
#include "NightOwl/Component/Concrete/Transform.h"
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

	auto shader = NightOwl::Graphics::RenderApi::CreateShader("Test", "./assets/Shaders/Standard.vert", "./assets/Shaders/Standard.frag");

	auto texture = NightOwl::Graphics::RenderApi::CreateTexture2D("./assets/Textures/The_Last_Of_Us.jpg");

	const std::vector vertices = {
		 NightOwl::Math::Vec3F(0.5f,  0.5f, 0.0f),
		 NightOwl::Math::Vec3F(0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f,  0.5f, 0.0f)
	};

	const std::vector colors = {
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 1.0f)
	};

	const std::vector triangles = {
		NightOwl::Math::Vec3UI(0,  1, 3),  // first Triangle
		NightOwl::Math::Vec3UI(1,  2, 3)  // second Triangle
	};

	const std::vector uvs = {
		 NightOwl::Math::Vec2F(0.0f,  0.0f),
		 NightOwl::Math::Vec2F(0.0f, 1.0f),
		 NightOwl::Math::Vec2F(1.0f, 0.0f),
		 NightOwl::Math::Vec2F(1.0f,  1.0f)
	};

	NightOwl::GameObject::GameObject cube("Cube");
	cube.AddComponent<NightOwl::Component::Mesh>();

	NightOwl::GameObject::GameObject cube2("Cube2");
	cube2.AddComponent<NightOwl::Component::Mesh>();

	NightOwl::GameObject::GameObject cube3("Cube3");
	cube3.AddComponent<NightOwl::Component::Mesh>();

	NightOwl::GameObject::GameObject camera("Camera");
	camera.AddComponent<NightOwl::Component::Camera>();



	camera.GetTransform().Translate(NightOwl::Math::Vec3F::Back(), NightOwl::Component::Space::World);

	//NightOwl::Component::Transform& trans = cube.GetTransform();
	//trans.Translate(0, 0, 0, NightOwl::Component::Space::Local);

	auto mesh = cube.GetComponent<NightOwl::Component::Mesh>();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetUVs(uvs);
	mesh->SetTriangles(triangles);

	mesh = cube2.GetComponent<NightOwl::Component::Mesh>();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetTriangles(triangles);

	mesh = cube3.GetComponent<NightOwl::Component::Mesh>();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetTriangles(triangles);

	cube.GetTransform().SetParent(cube2.GetTransform());
	cube3.GetTransform().SetParent(cube.GetTransform());

	cube.GetTransform().Translate(2.5, 0, 0, NightOwl::Component::Space::Local);
	cube3.GetTransform().Translate(2, 0, 0, NightOwl::Component::Space::Local);

	camera.GetTransform().Translate(0, 0, 2.5, NightOwl::Component::Space::Local);
	NightOwl::Math::Vec4F color(0.2f, 0.3f, 0.3f, 1.0f);

	while(!NightOwl::Window::WindowApi::GetWindow()->ShouldWindowClose())
	{
		cube.GetTransform().Rotate(0, 0, 0, NightOwl::Component::Space::Local);
		cube3.GetTransform().Rotate(0, 0, 1, NightOwl::Component::Space::World);
		cube2.GetTransform().Rotate(0, 0, 0, NightOwl::Component::Space::Local);

		NightOwl::Graphics::RenderApi::GetContext()->ClearColor(color);
		NightOwl::Graphics::RenderApi::GetContext()->ClearBuffer();


		shader->Bind();
		shader->SetUniformMat4F(cube.GetTransform().GetWorldMatrix(), "modelMatrix");
		shader->SetUniformMat4F(camera.GetComponent<NightOwl::Component::Camera>()->ViewProjectionMatrix(), "viewProjectionMatrix");
		texture->Bind(0);
		shader->SetUniformInt(texture->GetTextureId(), "inputTexture");
		cube.GetComponent<NightOwl::Component::Mesh>()->Bind();
		NightOwl::Graphics::RenderApi::GetContext()->DrawIndexed(NightOwl::Graphics::DrawType::Triangles, cube.GetComponent<NightOwl::Component::Mesh>()->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		cube.GetComponent<NightOwl::Component::Mesh>()->Unbind();
		texture->Unbind();
		shader->Unbind();

		shader->Bind();
		shader->SetUniformMat4F(cube2.GetTransform().GetWorldMatrix(), "modelMatrix");
		shader->SetUniformMat4F(camera.GetComponent<NightOwl::Component::Camera>()->ViewProjectionMatrix(), "viewProjectionMatrix");
		cube2.GetComponent<NightOwl::Component::Mesh>()->Bind();
		NightOwl::Graphics::RenderApi::GetContext()->DrawIndexed(NightOwl::Graphics::DrawType::Triangles, cube2.GetComponent<NightOwl::Component::Mesh>()->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		cube2.GetComponent<NightOwl::Component::Mesh>()->Unbind();
		shader->Unbind();

		shader->Bind();
		shader->SetUniformMat4F(cube3.GetTransform().GetWorldMatrix(), "modelMatrix");
		shader->SetUniformMat4F(camera.GetComponent<NightOwl::Component::Camera>()->ViewProjectionMatrix(), "viewProjectionMatrix");
		cube2.GetComponent<NightOwl::Component::Mesh>()->Bind();
		NightOwl::Graphics::RenderApi::GetContext()->DrawIndexed(NightOwl::Graphics::DrawType::Triangles, cube3.GetComponent<NightOwl::Component::Mesh>()->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		cube2.GetComponent<NightOwl::Component::Mesh>()->Unbind();
		shader->Unbind();

		NightOwl::Window::WindowApi::GetWindow()->Update();
	}
		
	return 0;
}
