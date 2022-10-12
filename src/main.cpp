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

	NightOwl::GameObject::GameObject obj("test object");
	obj.AddComponent<NightOwl::Component::Mesh>();
	obj.RemoveComponent<NightOwl::Component::Mesh>();
	obj.RemoveComponent<NightOwl::Component::Mesh>();

	NightOwl::GameObject::GameObject cameraObj("Camera");
	cameraObj.AddComponent<NightOwl::Component::Camera>();

	cameraObj.GetTransform().Translate(NightOwl::Math::Vec3F::Back(), NightOwl::Component::Space::World);
	auto camera = cameraObj.GetComponent<NightOwl::Component::Camera>();

	NightOwl::Component::Transform& trans = obj.GetTransform();

	auto mesh = obj.GetComponent<NightOwl::Component::Mesh>();
	mesh->SetVertices(vertices);
	mesh->SetColors(colors);
	mesh->SetTriangles(triangles);

	NightOwl::Math::Vec4F color(0.2f, 0.3f, 0.3f, 1.0f);

	while(!NightOwl::Window::WindowApi::GetWindow()->ShouldWindowClose())
	{
		obj.GetTransform().Rotate(0.0f, 0.0f, 1.0f, NightOwl::Component::Space::World);
		NightOwl::Graphics::RenderApi::GetContext()->ClearColor(color);
		NightOwl::Graphics::RenderApi::GetContext()->ClearBuffer();


		shader->Bind(); 
		shader->SetUniformMat4F(trans.GetWorldMatrix(), "modelMatrix");
		shader->SetUniformMat4F(cameraObj.GetComponent<NightOwl::Component::Camera>()->ViewProjectionMatrix(), "viewProjectionMatrix");
		mesh->Bind();
		NightOwl::Graphics::RenderApi::GetContext()->DrawIndexed(NightOwl::Graphics::DrawType::Triangles, obj.GetComponent<NightOwl::Component::Mesh>()->GetTriangles().size() * sizeof(mesh->GetTriangles()[0]));
		mesh->Unbind();
		shader->Unbind();

		NightOwl::Window::WindowApi::GetWindow()->Update();
	}
		
	return 0;
}
