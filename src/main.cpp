#include <iostream>

#include "NightOwl/Core/Components/Mesh.h"
#include "NightOwl/Core/Components/Transform.h"
#include "NightOwl/Core/Utitlity/GlErrorCheck.h"
#include "NightOwl/Math/Math.h"
#include "NightOwl/Core/Utitlity/Logging/LoggerManager.h"
#include "NightOwl/Graphics/RenderApi.h"
#include "NightOwl/Window/WindowApi.h"

int main()
{
	NightOwl::Math::Vec3F a(1, 2, 3);
	NightOwl::Utility::LoggerManager::Init();

	auto window = NightOwl::Window::WindowApi::CreateWindow("Hi", 600, 800);

	auto shader = NightOwl::Graphics::RenderApi::CreateShader("Test", "./assets/Shaders/entityShader.vs", "./assets/Shaders/entityShader.fs");

	std::vector vertices = {
		 NightOwl::Math::Vec3F(0.5f,  0.5f, 0.0f),
		 NightOwl::Math::Vec3F(0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f, -0.5f, 0.0f),
		 NightOwl::Math::Vec3F(-0.5f,  0.5f, 0.0f)
	};

	std::vector colors = {
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 0.0f),
		NightOwl::Math::Vec3F(0.0f,  1.0f, 1.0f)
	};

	std::vector triangles = {
		NightOwl::Math::Vec3UI(0,  1, 3),  // first Triangle
		NightOwl::Math::Vec3UI(1,  2, 3)  // second Triangle
	};

	NightOwl::Components::Mesh mesh;
	mesh.SetVertices(vertices);
	mesh.SetColors(colors);
	mesh.SetTriangles(triangles);

	NightOwl::Components::Transform transform;


	while(!window->ShouldWindowClose())
	{
		GL_CALL(glClearColor, 0.2f, 0.3f, 0.3f, 1.0f);
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT);


		shader->Bind();
		shader->SetUniformMat4F(transform.GetLocalModelMatrix(), "model");
		mesh.Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mesh.Unbind();
		shader->Unbind();

		window->Update();
	}
		
	return 0;
}
