#include "NightOwlPch.h"

#include "Graphics.h"
#include "NightOwl/Window/WindowApi.h"
#include "Types/GraphicsFormat.h"

namespace NightOwl
{
	void Graphics::Initialize()
	{
		IWindow* window = WindowApi::GetWindow().get();
		CreateContext(window);

		// Create G-Buffer and add initial position color and depth buffers
		deferredGBuffer.reset(reinterpret_cast<IRenderTexture*>(new OpenGlRenderTexture(window->GetHeight(), 
																						window->GetWidth(),
																						TextureFilterMode::Point, 
																						GraphicsFormat::RGBA16F,
																						GraphicsFormat::Depth32F)));
		// add normal color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA16F);
		// add color + specular color buffer
		deferredGBuffer->AddColorAttachment(GraphicsFormat::RGBA8);
	}

	void Graphics::Render()
	{
		// TODO: create steps for the g-buffer passes
	}

	void Graphics::Shutdown()
	{
		deferredGBuffer.release();
	}
}
