#include <iostream>

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

	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	std::vector attributesList = {
		NightOwl::Graphics::VertexBufferData("Position", NightOwl::Graphics::VectorFloat3),
		NightOwl::Graphics::VertexBufferData("Color", NightOwl::Graphics::VectorFloat3)
	};

	NightOwl::Graphics::VertexBufferLayout layout(attributesList);

	auto vertexBuffer = NightOwl::Graphics::RenderApi::CreateVertexBuffer(vertices, sizeof(vertices));
	vertexBuffer->SetVertexBufferLayout(layout);
	auto indexBuffer = NightOwl::Graphics::RenderApi::CreateIndexBuffer(indices, sizeof(vertices) / sizeof(unsigned int));
	auto vao = NightOwl::Graphics::RenderApi::CreateVertexArrayObject();
	vao->SetIndexBuffer(indexBuffer);
	vao->SetVertexBuffer(vertexBuffer);

	while(!window->ShouldWindowClose())
	{
		GL_CALL(glClearColor, 0.2f, 0.3f, 0.3f, 1.0f);
		GL_CALL(glClear, GL_COLOR_BUFFER_BIT);

		shader->Bind();
		vao->Bind();
		GL_CALL(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		vao->Unbind();
		shader->Unbind();

		window->Update();
	}
		
	return 0;
}
